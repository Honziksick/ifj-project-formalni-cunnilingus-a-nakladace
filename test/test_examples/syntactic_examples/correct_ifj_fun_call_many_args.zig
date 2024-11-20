const ifj = @import("ifj24.zig");
pub fn main() void {
    const var_with_very_long_identifier : ?f64 = 2.71;
    ifj.foo(10, 3.14, var_with_very_long_identifier, null, "Please work");
    var assign_the_monster = ifj.foo(10, 3.14, var_with_very_long_identifier, null, "Please work");
}