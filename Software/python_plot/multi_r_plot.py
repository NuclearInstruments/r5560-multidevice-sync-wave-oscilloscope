import pandas
import statistics
import matplotlib.pyplot as plt
import numpy as np
def zero_bl(data):
    
    d= np.concatenate(data.values)

    a = statistics.mean(d[0:50])
    return d-a


data_0 = pandas.read_fwf('w1.txt',  header=None, widths  =[8,8,8,8],colClasses= "integer")
data_1 = pandas.read_fwf('w2.txt',  header=None, widths  =[8,8,8,8],colClasses= "integer")
data_2 = pandas.read_fwf('w3.txt',  header=None, widths  =[8,8,8,8],colClasses= "integer")
data_3 = pandas.read_fwf('w4.txt',  header=None, widths  =[8,8,8,8],colClasses= "integer")



pandas.set_option('display.max_rows', None)


plt.plot(zero_bl(data_0[[0]]), color="blue")
plt.plot(zero_bl(data_0[[1]]), color="blue")
plt.plot(zero_bl(data_0[[2]]), color="blue")
plt.plot(zero_bl(data_0[[3]]), color="blue")

plt.plot(zero_bl(data_1[[0]]), color="red")
plt.plot(zero_bl(data_1[[1]]), color="red")
plt.plot(zero_bl(data_1[[2]]), color="red")
plt.plot(zero_bl(data_1[[3]]), color="red")

plt.plot(zero_bl(data_2[[0]]), color="green")
plt.plot(zero_bl(data_2[[1]]), color="green")
plt.plot(zero_bl(data_2[[2]]), color="green")
plt.plot(zero_bl(data_2[[3]]), color="green")

plt.plot(zero_bl(data_3[[0]]), color="magenta")
plt.plot(zero_bl(data_3[[1]]), color="magenta")
plt.plot(zero_bl(data_3[[2]]), color="magenta")
plt.plot(zero_bl(data_3[[3]]), color="magenta")


plt.show()