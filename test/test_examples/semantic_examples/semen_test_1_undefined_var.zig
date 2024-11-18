// Program: Nedefinovana promenna
//ERR3

const ifj = @import("ifj24.zig");
pub fn main() void {
    ifj.write("johA\n");
    const a: i32 = 12;

    ifj.write(a);
    ifj.write(b);   //Vypisovani nedefinovane promenne 'b'
}