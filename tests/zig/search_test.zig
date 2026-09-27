const std = @import("std");
const search = @import("codebase").search;

test "linear search returns the first matching index" {
    const values = [_]i32{ std.math.minInt(i32), -2, 7, -2, std.math.maxInt(i32) };

    try std.testing.expectEqual(@as(?usize, 1), search.linearSearchI32(&values, -2));
    try std.testing.expectEqual(@as(?usize, 4), search.linearSearchI32(&values, std.math.maxInt(i32)));
}

test "linear search returns null for empty or missing values" {
    const empty = [_]i32{};
    const values = [_]i32{ -3, 0, 8 };

    try std.testing.expectEqual(@as(?usize, null), search.linearSearchI32(&empty, 1));
    try std.testing.expectEqual(@as(?usize, null), search.linearSearchI32(&values, 7));
}

test "binary search finds boundaries and a duplicate" {
    const values = [_]i32{ std.math.minInt(i32), -2, -2, 0, 7, std.math.maxInt(i32) };

    try std.testing.expectEqual(@as(?usize, 0), search.binarySearchI32(&values, std.math.minInt(i32)));
    try std.testing.expectEqual(@as(?usize, values.len - 1), search.binarySearchI32(&values, std.math.maxInt(i32)));

    const duplicate_index = search.binarySearchI32(&values, -2);
    try std.testing.expect(duplicate_index != null);
    try std.testing.expectEqual(@as(i32, -2), values[duplicate_index.?]);
}

test "binary search handles empty, single, and missing values" {
    const empty = [_]i32{};
    const single = [_]i32{42};
    const values = [_]i32{ -4, 0, 5, 9 };

    try std.testing.expectEqual(@as(?usize, null), search.binarySearchI32(&empty, 1));
    try std.testing.expectEqual(@as(?usize, 0), search.binarySearchI32(&single, 42));
    try std.testing.expectEqual(@as(?usize, null), search.binarySearchI32(&single, 10));
    try std.testing.expectEqual(@as(?usize, null), search.binarySearchI32(&values, 6));
}
