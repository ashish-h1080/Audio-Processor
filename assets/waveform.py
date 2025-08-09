import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv(r"assets/waveform.csv")

plt.figure(figsize=(10, 4))
plt.plot(df["Time"], df["Amplitude"], linewidth=0.5)

plt.title("Waveform")
plt.xlabel("Time (seconds)")
plt.ylabel("Amplitude")
plt.grid(True)
plt.tight_layout()
plt.show()
