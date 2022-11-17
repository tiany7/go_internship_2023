cc_binary(
    name = "hello_server",
    srcs = ["http_server.cpp"],
    defines = ["CPPHTTPLIB_OPENSSL_SUPPORT"],
    visibility = ["//visibility:public"],
    deps = ["@comm_github_mysql_cpp_connector//:httplib",
            "@com_github_grpc_grpc//:grpc++",
            "@com_github_grpc_grpc//:grpc++_reflection",
            "//simple_calculator_demo/simple_adder:adder_cc_grpc"],
    linkopts = ["-lpthread"],
)