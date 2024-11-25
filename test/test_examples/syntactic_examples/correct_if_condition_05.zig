const ifj = @import("ifj24.zig");

pub fn main() void {
    var x: i32 = 32;
    const y : i32 = 89;

    if(x/((x+y)*8))
    {
        processItem(1458242);
    }
    else
    {
        _ = handleOddItem(53878);
    }
}