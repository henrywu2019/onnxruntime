import onnxruntime as ort
import numpy as np
import time
from hiq.framework.onnxruntime import plot_ort_profile


print(ort.__file__)

model_file_path = 'my.ort'
so = ort.SessionOptions()
so.enable_profiling = True
sess = ort.InferenceSession(model_file_path, sess_options=so, providers=['CPUExecutionProvider']) # 'CUDAExecutionProvider'

if sess:
    n = np.load('../x.npy')
    output = None
    start = time.monotonic()



    print("*"*80)
    r = sess.run(output, {'x': n})
    print("*"*80)



    print(time.monotonic() - start)
    print(r[0].shape)

    pf = sess.end_profiling()
    print(pf)
    plot_ort_profile(pf)
