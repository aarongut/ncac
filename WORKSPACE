new_local_repository(
    name = "system_include",
    path = "/usr/lib",
    build_file_content = """
cc_library(
    name = "curses",
    srcs = ["libcurses.dylib"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "curl",
    srcs = ["libcurl.dylib"],
    visibility = ["//visibility:public"],
)
    """,
)

new_http_archive(
    name = "gtest",
    url = "https://github.com/google/googletest/archive/release-1.8.0.tar.gz",
    sha256 = "58a6f4277ca2bc8565222b3bbd58a177609e9c488e8a72649359ba51450db7d8",
    build_file = "gtest.BUILD",
    strip_prefix = "googletest-release-1.8.0",
)
