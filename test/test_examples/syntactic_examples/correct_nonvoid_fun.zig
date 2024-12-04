const ifj = @import("ifj24.zig");

pub fn main() void {
    
}

pub fn foo_1() i32 {
    return 42;
}

pub fn foo_2() ?i32 {
    return null;
}

pub fn foo_3() f64 {
    return 3.14;  
}

pub fn foo_4() ?f64 {
    return null;
}

pub fn foo_5() []u8 {
    return ifj.string("hello");
}

pub fn foo_6() ?[]u8 {
    return null;
}