// Syntax error: missing "if" keyword

const ifj = @import("ifj24.zig");

pub fn main() void {
    var x: i32 = 32;
    const y : i32 = 89;

    (y < x)
    {
        processItem(1458242);
    }
    else
    {
        _ = handleOddItem(53878);
    }
}