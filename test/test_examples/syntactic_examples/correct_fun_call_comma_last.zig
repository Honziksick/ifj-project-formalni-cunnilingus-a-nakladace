const ifj = @import("ifj24.zig");
pub fn main() void {
    foo("Somebody left comma after me!",);
    bar(1024, "Somebody left comma after me, AGAIN!",);
    const y = foo("Somebody left comma after me!",);
    var z = bar(1024, "Somebody left comma after me, AGAIN!",);
}
