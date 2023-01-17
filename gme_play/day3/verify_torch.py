import numpy as np
import torch

i = np.load('Conv_nchwc_gme_149_input.npy')
n1, c1, h1, w1 = i.shape # 1, 256, 400, 296
w = np.load('Conv_nchwc_gme_149_weight.npy')
print(w.shape) # (64, 256, 3, 3) K=64 C=256 R=3 S=3
o = np.load('Conv_nchwc_gme_149_output.npy')
n2, c2, h2, w2 = o.shape # 1, 64, 400, 296

# torch.from_numpy()

# in_channels, out_channels, kernel_size, stride=1,
#                  padding=0, dilation=1, groups=1,
#                  bias=True, padding_mode='zeros', qconfig=None,
#                  device=None, dtype=None
conv2d = torch.nn.Conv2d(in_channels=c1,
                         out_channels=c2,
                         kernel_size=(3, 3),
                         stride=(1, 1),
                         padding=(1, 1),
                         dilation=(1, 1),
                         groups=1,
                         bias=False,
                         dtype=torch.float32)
conv2d.weight = torch.nn.Parameter(torch.from_numpy(w))
conv2d.zero_grad()
o_hat = conv2d(torch.from_numpy(i))
x = o_hat.detach().numpy()
print(o_hat.shape) # torch.Size([1, 64, 400, 296])
#print(o_hat - torch.from_numpy(o))
print(o_hat[0][0][0][:3]) # tensor([-15.3103,  55.9210,  -5.6894], grad_fn=<SliceBackward0>)