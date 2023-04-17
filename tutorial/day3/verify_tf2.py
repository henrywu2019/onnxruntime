import numpy as np
import tensorflow as tf

i = np.load('Conv_nchwc_gme_149_input.npy')
n1, c1, h1, w1 = i.shape
w = np.load('Conv_nchwc_gme_149_weight.npy')
print(w.shape) # (64, 256, 3, 3)
o = np.load('Conv_nchwc_gme_149_output.npy')
n2, c2, h2, w2 = o.shape

# torch.from_numpy()
weight=tf.convert_to_tensor(w, dtype=tf.float32)
weight = tf.transpose(weight, [2, 3, 1, 0])
print("weight shape:", weight.shape) # 3, 3, 256, 64
conv_layer = tf.keras.layers.Conv2D(filters=64, # K=64
                                    kernel_size=(3, 3),
                                    strides=(1, 1),
                                    dilation_rate=(1, 1),
                                    groups=1,
                                    padding='same',
                                    use_bias=False,
                                    data_format='channels_last',
                                    name='conv_layer')

conv_layer.kernel = tf.Variable(weight)
ti = tf.convert_to_tensor(i, dtype=tf.float32)

ti = tf.transpose(ti, [0, 2, 3, 1])

o_hat = conv_layer(ti)

o_hat2 = tf.transpose(o_hat, [0, 3, 1, 2])

#o_hat = conv2d(torch.from_numpy(i))
print(o_hat2.shape) # (1, 64, 400, 296)
print(o_hat2[0][0][0][:3]) # tf.Tensor([25.298512  12.396784  -3.7985535], shape=(3,), dtype=float32)
to = tf.convert_to_tensor(o, dtype=tf.float32)
print(sum(o_hat2 - to))

