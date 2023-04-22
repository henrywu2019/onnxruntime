import pandas as pd
import matplotlib.pyplot as plt

plt.style.use('ggplot')

# Load data from file
df = pd.read_csv("result.csv")
df = df[df['K'] <= 100]


fig, ax = plt.subplots(figsize=(18, 10))

# Extract t, K, and f columns for f = 1 and f = 2
t_f2 = df.loc[(df['f'] == 2) & (df['S']==32), 't']
K_f2 = df.loc[(df['f'] == 2) & (df['S']==32), 'K']
t_f1 = df.loc[df['f'] == 1, 't']
K_f1 = df.loc[df['f'] == 1, 'K']

t_f3 = df.loc[(df['f'] == 2) & (df['S']==16), 't']
K_f3 = df.loc[(df['f'] == 2) & (df['S']==16), 'K']

#t_f4 = df.loc[(df['f'] == 2) & (df['S']==12), 't']
#K_f4 = df.loc[(df['f'] == 2) & (df['S']==12), 'K']

# Create plot
plt.plot(K_f1, t_f1, "o-", color="blue", label="f=1")
plt.plot(K_f2, t_f2, "o-", color="red", label="f=2 S=32")
plt.plot(K_f3, t_f3, "x-", color="green", label="f=2 S=16")
#plt.plot(K_f4, t_f4, "v-", color="purple", label="f=2 S=12")
plt.xlabel("K")
plt.ylabel("t (us)")
plt.title("t vs. K for f=2 GME and f=1 ORT, Ci=256,H=800,W=34,K=3x3")
plt.legend()

plt.savefig("plot.png")
