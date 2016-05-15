#define proc 5

typedef LNode {
  byte next;
  byte waiting;
}

LNode mynode[proc+1];
byte n;
byte criticalSection[proc];
byte tail;
byte temp;
byte counter;
byte pred;

inline acquire() {
    mynode[_pid + 1].next = 0;

    //error when setting pred to mynode
    //pred = mynode[_pid+1];

    //swap
    d_step {
      temp = tail;
      tail = pred;
      pred = temp;
    }
    if
    :: pred ->
      mynode[_pid+1].waiting = 1;
      mynode[_pid+1].next = _pid + 1;
      mynode[_pid+1].waiting = 0;
    fi;
}

inline release() {
  byte i;
  if
  ::  mynode[_pid+1].next == 0 ->
    //compare and swap
    d_step {
      i = pred;
      pred = (pred == _pid + 1) -> 0 : pred); //looked up cas algo
    }
    if
    :: i == _pid+1;
      acquire()
    ::else ->
      skip;
    fi;
    (mynode[_pid+1].next == 0) //await
    mynode[_pid+1].waiting = 0;
}

active[proc] proctype p() {
  do
  :: counter < 50 ->
    acquire();
    counter++;
    release();
  od;

}

ltl claim { always eventually counter == 334}
