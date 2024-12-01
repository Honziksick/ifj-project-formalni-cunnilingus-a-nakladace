const ifj = @import("ifj24.zig");
pub fn main() void {
    const var_with_very_long_identifier = ifj.string("2.71");
    const nullable : []u8 = ifj.string("null");
    const pi = ifj.string("3.14");
    const ten = ifj.string("10");
    const assign_the_monster =
ifj.concat(ifj.string(ten), (ifj.concat(
                                       ifj.string(pi), (ifj.concat(
                                                                   ifj.string(var_with_very_long_identifier), ifj.concat(
                                                                                                                         ifj.string(nullable), ifj.string("Please work")                                        )          )               )          )                                                     )
          );
    ifj.write(assign_the_monster);
}

