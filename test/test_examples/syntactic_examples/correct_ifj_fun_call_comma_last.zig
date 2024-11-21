const ifj = @import("ifj24.zig");
pub fn main() void {
    ifj.foo("Somebody left comma after me!",);
    ifj.bar(1024, "Somebody left comma after me, AGAIN!",);
    const y = ifj.foo("Somebody left comma after me!",);
    var z = ifj.bar(1024, "Somebody left comma after me, AGAIN!",);
}
