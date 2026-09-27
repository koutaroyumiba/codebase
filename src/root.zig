//! Root module for the Zig implementation of the codebase.

const std = @import("std");

test "root module loads" {
    try std.testing.expect(true);
}
