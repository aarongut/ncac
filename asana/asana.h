#ifndef ASANA_ASANA_H_
#define ASANA_ASANA_H_

#include <stdlib.h>

#include "../cJSON/cJSON.h"

enum ASANA_RESOURCE {
  WORKSPACE,
  USER,
  TASK,
  PROJECT,
};

typedef struct Resource {
  enum ASANA_RESOURCE type;
  char *gid;
  char *name;
} Resource;

typedef struct Workspace {
  enum ASANA_RESOURCE type;
  char *gid;
  char *name;
} Workspace;

typedef struct User {
  enum ASANA_RESOURCE type;
  char *gid;
  char *name;
  size_t workspaces_len;
  Workspace *workspaces;
} User;

typedef struct Task {
  enum ASANA_RESOURCE type;
  char *gid;
  char *name;
  char *notes;
} Task;

typedef struct Project {
  enum ASANA_RESOURCE type;
  char *gid;
  char *name;
  size_t tasks_len;
  Task *tasks;
} Project;


typedef int asana_err;
#define ASANA_ERR_OK 0
#define ASANA_ERR_FETCH 1
#define ASANA_ERR_PARSE 2
#define ASANA_ERR_UNKNOWN_TYPE 3

asana_err asana_parse(char *json, void *resource);
void **asana_parse_array(char *json, size_t item_size, size_t *items);

/**
 * Parsing methods to take a cJSON object and populate the Asana resource struct
 */
void asana_extract(cJSON *data, void *resource);
void asana_extract_resource(cJSON *json, Resource *resource);
void asana_extract_user(cJSON *json, User *user);
void asana_extract_workspace(cJSON *json, Workspace *workspace);
void asana_extract_task(cJSON *json, Task *task);
void asana_extract_project(cJSON *json, Project *project);

asana_err user_info(User *user);

asana_err user_task_list_gid(char *workspace_gid, char *gid);
asana_err user_task_list(char *task_list_gid, Project *task_list);

void asana_free_resource_array(Resource *resource, size_t len);
void asana_free_resource(Resource *resource);
size_t asana_resource_size(Resource *resource);

#endif // ASANA_ASANA_H_
