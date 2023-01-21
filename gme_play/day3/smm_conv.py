"""
/home/henry/miniconda3/envs/ocr-py38/bin/python /home/henry/wendy/git.repo/onnxruntime/gme_play/day3/smm_conv.py
torch.Size([1, 1, 6, 6])
0.0006800639966968447
tensor([[[[ 474.,  519.,  564.,  609.],
          [ 744.,  789.,  834.,  879.],
          [1014., 1059., 1104., 1149.],
          [1284., 1329., 1374., 1419.]]]])
0.001002063014311716
tensor([[[[ 474.,  519.,  564.,  609.],
          [ 744.,  789.,  834.,  879.],
          [1014., 1059., 1104., 1149.],
          [1284., 1329., 1374., 1419.]]]], dtype=torch.float64,
       grad_fn=<ConvolutionBackward0>)
"""
import numpy as np
import torch
import time

I = np.array([i for i in range(1, 37)], dtype=float).reshape(1, 1, 6, 6)
F = np.array(range(1, 10), dtype=float).reshape(1, 1, 3, 3)

n1, ci, h, w = I.shape
co, ci, kw, kh = F.shape
o = np.zeros(16, dtype=float).reshape(1, 1, 4, 4)
n1, co, h_, w_ = o.shape
ti = torch.from_numpy(I)
tw = torch.from_numpy(F)

print(ti.shape)
r = torch.zeros((1, co, h_, w_))

t0 = time.monotonic()

for c in range(ci):
    for j in range(kw):
        sliced_mat = ti[0][c][0:h, j:j+w_]
        #print(sliced_mat.shape)
        for k in range(kh):
            shifted_mat = sliced_mat[k:k+h_, :]
            for m in range(co):
                scalar = F[m][c][k][j]
                r[0][c] += scalar * shifted_mat

print(time.monotonic() - t0)
print(r)
t1 = time.monotonic()

conv2d = torch.nn.Conv2d(in_channels=ci,
                         out_channels=co,
                         kernel_size=(kh, kw),
                         stride=(1, 1),
                         padding=(0, 0),
                         dilation=(1, 1),
                         groups=1,
                         bias=False,
                         dtype=torch.float32)
conv2d.weight = torch.nn.Parameter(tw)
conv2d.zero_grad()
o_hat = conv2d(torch.from_numpy(I))
print(time.monotonic() - t1)
print(o_hat)
