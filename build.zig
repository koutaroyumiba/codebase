const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const codebase = b.addModule("codebase", .{
        .root_source_file = b.path("src/root.zig"),
        .target = target,
        .optimize = optimize,
    });

    const module_tests = b.addTest(.{
        .root_module = codebase,
    });
    const run_module_tests = b.addRunArtifact(module_tests);

    const search_tests = b.addTest(.{
        .root_module = b.createModule(.{
            .root_source_file = b.path("tests/zig/search_test.zig"),
            .target = target,
            .optimize = optimize,
            .imports = &.{
                .{ .name = "codebase", .module = codebase },
            },
        }),
    });
    const run_search_tests = b.addRunArtifact(search_tests);

    const test_step = b.step("test", "Run Zig tests");
    test_step.dependOn(&run_module_tests.step);
    test_step.dependOn(&run_search_tests.step);
}
