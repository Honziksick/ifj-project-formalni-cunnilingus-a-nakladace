const ifj = @import("ifj24.zig");
pub fn main() void {
    _ = foo(10);
    const ten_days_till_deadline = foo(10);
    ifj.write(ten_days_till_deadline);
}

pub fn foo(x: i32) i32 {
    return x;
}
