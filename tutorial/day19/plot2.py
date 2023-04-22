import pandas as pd
import matplotlib.pyplot as plt

plt.style.use('ggplot')
#plt.title("Memory and Latency (ZPZR vs. SOTA)")
# Load data from file
df = pd.read_csv("result.csv")
df = df[df['K'] <= 100]

# Create subplots with a shared y-axis
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 10), sharey=True)

# Plot the first subplot
d = df[(df['f'] == 2) & (df['S'] == 16)]
d2= df[(df['f'] == 1)]
ax1.plot(d['K'], d['m'], "o-", color="red", label="Memory ZPZR", alpha=0.6)
ax1.plot(d2['K'], d2['m'], "v-", color="green", label="Memory SOTA", alpha=0.6)
#ax1.set_xlabel("Kernel Batch Size(Out_Channel)")
ax1.set_ylabel("Memory Usage(KB)")
ax1.set_title("ZPZR vs. SOTA(Ci=256,H=800,W=34,Kernel=3x3,AMD EPYC 7742,2.2GHz,Cache 64/512/1.6), AVX2")
ax1.legend()

# Filter the data by f and K for the second subplot
t_f1 = df.loc[df['f'] == 1, 't']
K_f1 = df.loc[df['f'] == 1, 'K']
#t_f2 = df.loc[(df['f'] == 2) & (df['S']==32), 't']
#K_f2 = df.loc[(df['f'] == 2) & (df['S']==32), 'K']
t_f3 = df.loc[(df['f'] == 2) & (df['S']==16), 't']
K_f3 = df.loc[(df['f'] == 2) & (df['S']==16), 'K']

ax2.plot(K_f1, t_f1, "v-", color="green", label="Latency SOTA", alpha=0.6)
#ax2.plot(K_f2, t_f2, "o-", color="red", label="f=2 S=32")
ax2.plot(K_f3, t_f3, "o-", color="red", label="Latency ZPZR", alpha=0.6)
ax2.set_xlabel("Kernel Batch Size(Out_Channel)")
ax2.set_ylabel("Latency(microsecond)")
#ax2.set_title("Latency vs. Out_Channel (Ci=256,H=800,W=34,Kernel=3x3)")
ax2.legend()


plt.legend()

plt.savefig("zpzr_vs_sota.png")
