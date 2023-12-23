
Graph is composed of 4 branches
- **broadcaster** sends to **%bz**, **%cn**, **%bs** and **%rq**
- These in turn feeds into **&gk**, **&fv**, **&sl** and **&rt** respectively
- These are each part of 4 complex networks composed exclusively of flip-flop modules[%]
- These also each feed in a single-input conjunction module, i.e. inverters (resp. **&zk**, **&qs**, **&ks** and **&jf**)
- These then feed into **&hj** that must have all inputs high to output low to target **rx**

=> So **&gk**, **&fv**, **&sl** and **&rt** must each be LOW for **rx** to get a LOW signal


**hj** -> low iff **zk** -> high iff **gk** -> low
          iff **qs** -> high iff **fv** -> low
          iff **ks** -> high iff **sl** -> low
          iff **jf** -> high iff **rt** -> low


Example for **&sl**: all inputs must be high
     rxmqfjfcgjmd
     qdmlzckcvlrc
     =<>><><>>>>>

1.   100000000000 - **broadcaster** --low-> **%rq** [on]
2.   010000000000 - **broadcaster** --low-> **%rq** [off] --low-> **%xd** [on]
3.   110000000000 - **broadcaster** --low-> **%rq** [on]
4.   001000000000 - **broadcaster** --low-> **%rq** [off] --low-> **%xd** [off] --low--> **%mm** [on]

5.   101000000000 - **broadcaster** --low-> **%rq** [on]
6.   011000000000 - **broadcaster** --low-> **%rq** [off] --low-> **%xd** [on]
7.   111000000000 - **broadcaster** --low-> **%rq** [on]
8.   000100000000 - **broadcaster** --low-> **%rq** [off] --low-> **%xd** [off] --low--> **%mm** [off] --low--> **%ql** [on]

...

0.   0.11.1.11111
First one to occur:   rev(101101011111) = 4013
0.   101101011111 - **broadcaster** --low-> **%rq** [on] => **&sl** [all input high] -> send low to all outputs
     101101111111               --low-> **%fk** [on]
     001101111111               --low-> **%rq** [off]
     001111111111               --low-> **%fz** [on]
     011111111111               --low-> **%xd** [on]
     001111111111         **(%rq)** --low-> **%xd** [off]
     000111111111         **(%xd)** --low-> **%mm** [off]
     ...
     000000000001         **(%mr)** --low-> **%dc** [off]
     000000000000


# Summary

The four branches do the same thing: they implement a form of binary counter (each flip-flop acts as a 1-bit counter).
The number at which it resets is given by the links between the conjunction module linked to all flip-flop modules in the branch:
- if flip-flop -> conjunction, then this bit must be HIGH to activate the conjuction, i.e. this bit is a 1
- if conjunction -> flip-flop, then this bit will not have an effect on the conjunction activation, so it can be whatever,
    and in practice it will be a 0 (because it happens before it becomes a 1)

When the conjunction module is activated, it sends a low pulse to its inverter which feeds into the final conjunction module,
and it also sets all flip-flop that were at 0 (i.e. the ones that don't output to the conjunction) and then sends a final pulse
to the first flip-flop to reset them all in cascade.

In other words, this is a counter that counts to a limit where each of its bit is given by the nature of the link between the
corresponding flip-flop and the conjunction module.
In the end, as there are 4 such branches, we take the least-common multiplier of the period of each of these branches: that's our answer.

