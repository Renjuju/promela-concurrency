#define threads 6

byte nextTick;
byte serving;
byte criticalSection;

active[threads] proctype p() {
  byte mytick; //non-critical section

  do::serving < 250 ->
    d_step {
      mytick = nextTick; //mytick ← fetch-and-add(ticket, 1)
      nextTick = mytick + 1;
    }

    (mytick == serving); //await mytick = serving

    criticalSection++;//critical section: start
    assert(criticalSection == 1); //make sure only 1 process in cs
    criticalSection--; //critical section: end

    serving++; //serving ← mytick+1
  od
}
