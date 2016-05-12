# MicroOpt (HW9)
##### Alex Pan

####Layout
Nothing special about the `makefile` I just wanted to save a bit of time.
In `converter.c` I basically have kept most of my "successful" optimization attempts. Reading from the top-down, things get faster until `conv4`. After that, my attempts for `conv5` and `conv6` had some weird behavior that I will talk about below. In general I have included some minor comments throughout the code that point out a few places that I tried to make things faster.

####`conv` - `conv4`
Basically the general idea for optimzation was already laid out in `conv`, which was to iterate through a string, change its ascii into numbers, and then add things up to get the number. In `conv2` I started using bitshifts for the multiplication by 10 and logical operators on bytes to speed up converting from ascii to ints (since the offset nicely worked out to 48 or 0011 0000). I don't actually think this sped things up very much, if at all (I'll talk about this later in the results). In `conv3` I removed an iteration from the loop which, as expected, shaved off a ton of time. 
At this point, I built a helper function `counter` to confirm what the inputs looked like. ie: I wanted to see how many digits were in each number and found that all numbers fell between 3-5 digits in length. So improving on the idea of `conv3` with this assumption, `conv4` removes a bunch of the extra checks by taking out the for-loop and only adding in as many checks as necessary. This saves tons of time and appears to be my fastest function, running at `0.004929s` on my mac.

####`conv5` - `conv6`
These were my attempts at seeing if I could make things faster with assembly. `conv5` is pretty messy and was basically just scratch-work. `conv6` is pretty close in line with what I attempted to do with `conv4`. However, my issue with these two was that on `-O3` level of optimization, I got a compiler error. For some reason it didn't like my using the tag `end:` though I was unable to figure out why tags were not allowed in `-O3`. As such, I was only able to run `conv5` and `conv6` on `-O1` (as the same error appears on `-O2`). Surprisingly, even my shoddy work for `conv5` is comparable to the speed of `conv6` on this level of optimzation. 
Initially I was actually trying to do this with the SSE instruction set, but I found that it was very difficult to do - partially because this was before I had made `conv5` and `conv6`, but mainly because loading different length numbers into the registers makes things weird and difficult. `conv5` and `conv6` were basically intended to be my intermediary step towards figuring out how to vectorize this, except these are currently not working on `-O3` so I don't know how I will do the vectorized version since that would take even more conditionals.

####Testing/Results
Unfortunately, I have not yet had time to test these on polytopia. So I am actually not if these will compile on the linux machines, let alone how fast they will be. However, I did test things pretty thoroughly on my mac while I was making them. Here is a table of my general results.

##### Table of results: 
| Function| -O1 | -O3 |
| --- | --- | --- |
| `atoi` | slow* | 0.065832 |
| `conv` | 0.011262** | 0.01267 |
| `conv2` | 0.012855 | 0.011897 |
| `conv3` | 0.010558 | 0.009055 |
| `conv4` | 0.006452 | 0.004929 |
| `conv5` | 0.009286 | N/A |
| `conv6` | 0.009279 | N/A |

`* - I say slow because I didn't actually test it. Given how slow it was at -O3, I didn't want to wait around if it was actually slower on a lower optimization level.`

`** - I am highlighting this one because the -O1 test I ran was actually faster in one run than the -O3. More on this below.`

I was kind of curious about how my assembly code was running against the other versions, but I couldn't test it at `-O3` since that didn't compile, so I ended up just testing everything at `-O1`.

So, as you can see, some weird stuff happens at `-O1`. I'm guessing there is a bunch of noise, because I saw (but forgot to record - my bad!) some iterations of testing for `conv`, `conv2`, and `conv3` all running faster on `-O1` than `-O3`. I think by `conv4` the speedup from fewer conditionals makes enough of a difference where the background noise is no longer enough to create weird results like the one I highlighted (the `**`).

Also interesting to note, `conv5` and `conv6` are slow! Slow as in around the ballpark of `conv3`. This is pretty interesting since I tried to model my assembly code on my idea for `conv4`. One might say the lower optimization level is making it slower, and it probably is to some degree, but also note that `conv4` is still faster than `conv5` and `conv6` when optimized at `-O1`! My guess is that the many loads and unloads just take up too much time. 

#### Conclusions
`atoi` is very slow! Like a whole order of magnitude! But also, my optimization is tailored very specifically to this dataset. Even still, a more general version of this (which assumes you give it inputs that are actually numbers) like `conv` is still several times faster. I'm guessing that there are checks and other overhead in `atoi` that let it work in more general cases (or else why would we use it right?) but I did not find any "source code" for it (which was also kind of surprising. I probably just need to step up my Google game). Anyhow, for this is what I have at the moment. I will hopefully have an update by Friday. Thanks for reading.
