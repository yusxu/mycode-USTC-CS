const m = 7, n = 85;
var x, y, z, q, r;

procedure multiply;
var a, b;
begin
	a := x; b := y; z := 0;
	while b > 0 do
	begin
		if odd b then z := z + a;
		a := 2 * a; b := b / 2;
	end;
end;

procedure gcd;
var f, g;
begin
	f := x;
	g := y;
	while f != g do
	begin
		if f < g then g := g - f;
		if g < f then f := f - g;
	end;
end;

begin
	x := m; y := n; call multiply;
	x := 34; y := 36; call gcd;
end.

