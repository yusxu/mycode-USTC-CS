const m = 7, n = 85 /*miss token: ';'*/
var x, y, z, q r; /*miss token: ','*/
const ; /*miss successor: const ident*/

procedure ; /*miss successor: procedure ident*/
var a, b;
begin
	a := x; b := y; z := 0;
	while b > 0 do
	begin
		if odd b then z := z + a) * 1; /*matching error: left bracket*/
		a := 2 * a; b := b / 2;
	end;
end;

procedure gcd;
var ; /*miss successor: var ident*/
begin
	f x; /*miss successor: ident :=*/
	g := y;
	while f != g /*matching error: while do*/
	begin
		if f < g  g := g - f; /*matching error: if then*/
		if g < f then f := f - g;
end; /*matching error: begin end*/

begin
	x := m; y := n; call multiply;
	x := 34; y := 36; call gcd;
end /*miss token: '.'*/
