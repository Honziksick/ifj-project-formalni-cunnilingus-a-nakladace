const ifj = @import("ifj24.zig");
pub fn main() void {
    var vysl : i32 = 0;
    vysl = 65536;
    var habib : ?[]u8 = ifj.string("");
    habib = ifj.string("baba$Ja$Ga");
    var abus_babus : ?f64 = 3.14;
    abus_babus = null;
    ifj.write(ifj.string("#ifj.wrajt"));
    const a = ifj.f2i(vysl);
    const b = ifj.readi32();
    const c = ifj.string("#ifj.stryng");
    const d = ifj.strcmp(habib, abus_babus);
    ifj.write(
        ifj.concat(
            ifj.string(a), 
            ifj.concat(
                ifj.string(b), 
                c
            )
        ), 
        ifj.string(d)
);
}