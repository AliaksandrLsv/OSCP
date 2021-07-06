# Project : The museum 
***
At instance T, we suppose that :
 - X(T) : Number of visitors in the hall A.
 - Y(T) : Number of visitors in the hall B.
 - N (T): Number of  all the visitors since t=0 to T.
So, at instance T+1 :
 - X(T+1) = X(T) - AB + BA + U - Z
     * AB : Visitors who leave A to B.
     * BA : Visitors who return to A from B.
     * Z : Visitors whe leave the museum.
     * U : New visitors.
 - Y(T+1) = Y(T) + AB - BA
 - N(T+1) = N(T) + U

We create two threads which manage each hall separately :
  - thread_A : executes the function fn_A updates number of visitors in the hall A and the number N.
  - thread_B : executes the function fn_B updates number of visitors in the hall B.
