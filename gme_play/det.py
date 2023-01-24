import onnxruntime as ort
import numpy as np
import time
from hiq.framework.onnxruntime import plot_ort_profile


print(ort.__file__)

model_file_path = 'det_doc.onnx'
model_file_path = 'det_doc.ort'
#model_file_path = 'day2/modified.onnx'
so = ort.SessionOptions()
#so.optimized_model_filepath = "/tmp/modified.ort"
so.enable_profiling = False
sess = ort.InferenceSession(model_file_path, sess_options=so, providers=['CPUExecutionProvider']) # 'CUDAExecutionProvider'

if sess:
    # 1,3,1600,1184
    n = np.load('x.npy')
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

