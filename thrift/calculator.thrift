namespace cpp Calculator

struct AddRequest {
  1: i64 num1;
  2: i64 num2;
}

service CalculatorService {
   i64 Add (1:AddRequest request)
}
