#include "asana.h"

#include <string.h>
#include <stdio.h>

#include "../cJSON/cJSON.h"
#include "fetch.h"

asana_err asana_parse(char *json, void *resource) {
  if (json == NULL) {
    return ASANA_ERR_PARSE;
  }

  asana_err ret = ASANA_ERR_PARSE;

  cJSON *parsed = cJSON_Parse(json);

  if (parsed != NULL) {
    cJSON *data = cJSON_GetObjectItemCaseSensitive(parsed, "data");
    if (cJSON_IsObject(data)) {
      asana_extract(data, resource);
      ret = ASANA_ERR_OK;
    }
  }

  cJSON_Delete(parsed);
  return ret;
}

void asana_extract(cJSON *data, void *resource) {
  cJSON *resource_type = cJSON_GetObjectItemCaseSensitive(data, "resource_type");

  if (cJSON_IsString(resource_type)) {
    char *typ = resource_type->valuestring;

    fprintf(stderr, "Parsing a %s\n", typ);

    if (strcmp(typ, "user") == 0) {
      asana_extract_user(data, resource);
    } else if (strcmp(typ, "workspace") == 0) {
      asana_extract_resource(data, resource);
    } else if (strcmp(typ, "task") == 0) {
      asana_extract_task(data, resource);
    } else if (strcmp(typ, "project") == 0 || strcmp(typ, "user_task_list") == 0) {
      asana_extract_project(data, resource);
    } else {
      fprintf(stderr, "Unknown resource type: %s\n", typ);
    }
  }
}


void **asana_parse_array(char *json, size_t item_size, size_t *items) {
  if (json == NULL) {
    return NULL;
  }

  void *ret = NULL;
  cJSON *parsed = cJSON_Parse(json);

  if (parsed != NULL) {
    cJSON *data = cJSON_GetObjectItemCaseSensitive(parsed, "data");
    if (cJSON_IsArray(data)) {
      *items = cJSON_GetArraySize(data);
      ret = calloc(item_size, *items);

      cJSON *item = NULL;
      size_t i=0;
      cJSON_ArrayForEach(item, data) {
        asana_extract(item, (char *)ret+(i*item_size));
        i++;
      }
    }
  }

  cJSON_Delete(parsed);
  return (void **)ret;
}

void asana_extract_user(cJSON *json, User *user) {
  user->type = USER;
  asana_extract_resource(json, (Resource *)user);

  fprintf(stderr, "Parsing one user. Name: %s\n", user->name);

  cJSON *workspaces = cJSON_GetObjectItemCaseSensitive(json, "workspaces");
  if (cJSON_IsArray(workspaces)) {
    fprintf(stderr, "We got workspaces\n");
    user->workspaces_len = cJSON_GetArraySize(workspaces);
    user->workspaces = calloc(sizeof(Workspace), user->workspaces_len);

    cJSON *workspace = NULL;
    size_t i = 0;
    cJSON_ArrayForEach(workspace, workspaces) {
      fprintf(stderr, "Extracting workspace\n");
      asana_extract_resource(workspace, (Resource *)&(user->workspaces[i]));
      i++;
    }
  } else {
    user->workspaces_len = 0;
    user->workspaces = NULL;
  }
}

void asana_extract_task(cJSON *json, Task *task) {
  task->type = TASK;
  asana_extract_resource(json, (Resource *)task);

  cJSON *notes = cJSON_GetObjectItemCaseSensitive(json, "notes");
  if (cJSON_IsString(notes)) {
    task->notes = malloc(sizeof(char)*strlen(notes->valuestring));
    strcpy(task->notes, notes->valuestring);
  } else {
    task->notes = NULL;
  }
}

void asana_extract_project(cJSON *json, Project *project) {
  project->type = PROJECT;
  asana_extract_resource(json, (Resource *)project);

  cJSON *tasks = cJSON_GetObjectItemCaseSensitive(json, "tasks");
  if (cJSON_IsArray(tasks)) {
    project->tasks_len = cJSON_GetArraySize(tasks);
    project->tasks = calloc(sizeof(Task), project->tasks_len);

    cJSON *task = NULL;
    size_t i=0;
    cJSON_ArrayForEach(task, tasks) {
      asana_extract_task(task, &(project->tasks[i]));
      i++;
    }
  } else {
    project->tasks_len = 0;
    project->tasks = NULL;
  }
}

void asana_extract_resource(cJSON *json, Resource *resource) {
  cJSON *gid = cJSON_GetObjectItemCaseSensitive(json, "gid");
  if (cJSON_IsString(gid)) {
    resource->gid = malloc(sizeof(char)*strlen(gid->valuestring));
    strcpy(resource->gid, gid->valuestring);
    fprintf(stderr, "Extracted resource ID %s\n", resource->gid);
  } else {
    resource->gid = NULL;
  }

  cJSON *name = cJSON_GetObjectItemCaseSensitive(json, "name");
  if (cJSON_IsString(name)) {
    resource->name = malloc(sizeof(char)*strlen(name->valuestring));
    strcpy(resource->name, name->valuestring);
  } else {
    resource->name = NULL;
  }
}

asana_err user_info(User *user) {
  asana_err ret = ASANA_ERR_FETCH;

  Response *user_resp = asana_fetch("users/me");

  if (user_resp == NULL) {
    return ret;
  }

  if (user_resp->status == 200) {
    ret = asana_parse(user_resp->body, user);
  } else {
    fprintf(stderr, "Error fetching user: %d", user_resp->status);
  }

  asana_free_response(user_resp);

  return ret;
}

asana_err user_task_list_gid(char *workspace_gid, char *gid) {
  int ret = ASANA_ERR_FETCH;
  char path[ASANA_URL_MAX_LENGTH];
  snprintf(path, ASANA_URL_MAX_LENGTH, "users/me/user_task_list?workspace=%s&opt_fields=gid,resource_type", workspace_gid);
  Response *task_list_resp = asana_fetch(path);

  if (task_list_resp == NULL) {
    return ret;
  }

  if (task_list_resp->status == 200) {
    Project task_list;
    ret = asana_parse(task_list_resp->body, &task_list);

    if (ret == ASANA_ERR_OK) {
      strcpy(gid, task_list.gid);
      ret = ASANA_ERR_OK;
    }
  } else {
    fprintf(stderr, "Error fetching user_task_list: %d\n", task_list_resp->status);
  }

  asana_free_response(task_list_resp);

  return ret;
}

asana_err user_task_list(char *task_list_gid, Project *task_list) {
  asana_err ret = ASANA_ERR_FETCH;
  char path[ASANA_URL_MAX_LENGTH];
  snprintf(path, ASANA_URL_MAX_LENGTH, "user_task_lists/%s/tasks?limit=100&completed_since=now", task_list_gid);

  Response *task_list_resp = asana_fetch(path);

  if (task_list_resp == NULL) {
    return ret;
  }

  if (task_list_resp->status == 200) {
    task_list->tasks = (Task *)asana_parse_array(task_list_resp->body, sizeof(Task), &(task_list->tasks_len));
    ret = task_list->tasks == NULL ? ASANA_ERR_PARSE : ASANA_ERR_OK;
  } else {
    fprintf(stderr, "Error fetching user_task_list: %d\n",
            task_list_resp->status);
  }

  asana_free_response(task_list_resp);

  return ret;
}
