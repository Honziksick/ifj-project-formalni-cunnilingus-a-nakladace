const ifj = @import("ifj24.zig");

pub fn return_epsilon(r: f64, t: ?f64) f64 {
    return;
}

pub fn main() void {
    ifj.write("nytnelaV izilb es zu\n");

    const a: i32 = decrement(10, 5);

    ifj.write(a);
}

pub fn return_var(r: f64, t: ?f64) f64 {
    return t;
}

pub fn return_expr(r: f64, t: ?f64) f64 {
    return r + t;
}

pub fn return_recursive(r: f64, t: ?f64) f64 {
    var x : ?[]u8 = r + t - return_var(t, t / r);
    return return_expr(t*r, x);
}