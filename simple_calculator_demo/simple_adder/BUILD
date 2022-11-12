package(default_visibility = ["//visibility:public"])

load("@rules_proto//proto:defs.bzl", "proto_library")
load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")


proto_library(
    name = "adder_proto",
    srcs = ["adder.proto"],
)

cc_proto_library(
    name = "adder_cc_proto",
    deps = [":adder_proto"],
)

cc_grpc_library(
    name = "adder_cc_grpc",
    srcs = [":adder_proto"],
    grpc_only = True,
    deps = [":adder_cc_proto"],
)

cc_binary(
    name = "adder_client",
    srcs = ["adder_client.cpp"],
    defines = ["BAZEL_BUILD"],
    deps = [":adder_cc_grpc",
            "@com_github_grpc_grpc//:grpc++",],
)

cc_binary(
    name = "adder_server",
    srcs = ["adder_service.cpp"],
    defines = ["BAZEL_BUILD"],
    deps = [":adder_cc_grpc",
            "@com_github_grpc_grpc//:grpc++",
            "@com_github_grpc_grpc//:grpc++_reflection",],
)