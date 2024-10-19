### Not entirely sure whats the best way to save data on memory. If we have `c = a + b`, should that be saved as the computed value? or the symbols? When should stuff be computed. I think my brain wants to jump into immediately identifying the ideal layers/passes and its stressing me lmao.

### Started naively with the idea of saving the symbols, but the code didn't expand very well to support it as i would like. As of now memory either saves a single symbol, or an int.

## Input

```
a = 4
b = a
c = a + b
```

## Output

```
------------Memory-----------
b->a
c->8
a->4
------------Memory-----------
------------As Values-----------
b->4
c->8
a->4
```
