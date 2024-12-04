const ifj = @import("ifj24.zig");

pub fn return_epsilon(r: f64, t: ?f64) f64 {
    ifj.write(r);
    ifj.write(t);
    return 3.14;
}

pub fn main() void {

}

pub fn return_var(r: f64, t: ?f64) f64 {
    ifj.write(t);
    return r;
}

pub fn return_expr(r: f64, t: f64) f64 {
    return r + t;
}

pub fn return_recursive(r: f64, t: f64) f64 {
    var x : ?f64 = r + t - return_var(t, t / r);
    x = x;
    x = null;
    return return_expr(t*r, r);
}