procedure print(s: String)
is
begin
    Put_Line(s);
end print;

function rangeSum(rangeStart: Integer; rangeEnd: Integer) return Integer
is
    s: Integer;
    b: Integer;
    i: Integer;
begin
    s := 0;
    b := 0;
    for i in rangeStart .. rangeEnd loop
        s := s + i;
    end loop;
    return s;
end rangeSum;

procedure main() 
is 
    skibidi: array(1 .. 50) of Integer;
    gg: Integer;
    bombom: String; 
begin
    (1 + 2) * (3 - 4) / 5;
    if 1 + 2 = 5 and true or not false then
        print("Helloworld!");
    elsif false then
        print("Godbyeworld?");
    else
        gg := rangeSum(100, 1000);
        print("rangeSum(100, 1000)");
    end if;
end main;
