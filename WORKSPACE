new_local_repository(
    name = "system_include",
    path = "/usr/lib",
    build_file_content = """
cc_library(
    name = "curses",
    srcs = ["libcurses.dylib"],
    visibility = ["//visibility:public"],
)
    """,
)
