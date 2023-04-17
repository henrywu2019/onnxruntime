import numpy as np
import torch

i = np.load('Conv_nchwc_gme_149_input.npy')
n1, c1, h1, w1 = i.shape  # 1, 256, 400, 296
w = np.load('Conv_nchwc_gme_149_weight.npy')
K, C, R, S = w.shape
print(w.shape)  # (64, 256, 3, 3) K=64 C=256 R=3 S=3
o = np.load('Conv_nchwc_gme_149_output.npy')
n2, c2, h2, w2 = o.shape  # 1, 64, 400, 296

ti = torch.from_numpy(i)
tw = torch.from_numpy(w)

pad = torch.nn.ConstantPad2d((1, 1, 1, 1), 0)
r = torch.zeros((1, K, h1, w1))

batch=0
import time
for k in range(K): # output channel
    print(k, end=',')
    for input_channel in range(c1): # input channel
        #t0 = time.monotonic()
        p = pad(ti[batch][input_channel]) # padding
        w = tw[k][input_channel]
        #for i in range(h1): # input height
        #    for j in range(w1): # input width
        for i in range(9):
            for j in range(9):
                x = p[i:i + 3, j:j + 3]
                r[0][k][i][j] += torch.sum(torch.mul(x, w))
        #print("channel:", channel, ', latency:', time.monotonic()-t0)

#torch.save(r, 'result.pt')

print(r[0][0][0][:6]) # ([ -15.3103,   55.9210,   -5.6891,    3.1340, -120.4516,   97.7626])
