# packing small integers


There are a 1D array of small integers in range(0,7), can you pack them into another 1D array of integers so that the resulting array is as short as possible?

hint:
[2, 1, 2, 6, 2, 5, 7, 2, 5, 2, 3, 5, 1, 6, 3]


```
 ==> [-715215734, 3866] (int)
 ==> [3579751562, 3866] (uint)
```

```
>>> bin(3579751562)
'0b11 010 101 010 111 101 010 110 010 001 010'
                         32527526 1  2
>>> bin(3866)
'0b11 110 001 101 0'
  3    6   1   5
```
