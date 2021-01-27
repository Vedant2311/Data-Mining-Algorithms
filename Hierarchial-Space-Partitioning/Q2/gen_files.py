import numpy as np
import sys
import sklearn
from sklearn.decomposition import PCA

def main():
    if len(sys.argv) < 2:
        infileName = 'image_data.dat'
    else:
        infileName = sys.argv[1]
    
    data = np.loadtxt(infileName)
    # Generating reducded dimension files using PCA
    dimension_list = [2,4,8,10]
    for l in dimension_list:
        pca = PCA(n_components=l)
        fileOut = str(l) + "_pca"
        r = pca.fit_transform(data)
        np.random.shuffle(r)
        np.savetxt(fileOut,r[:-100,:],fmt='%f')
        np.savetxt(fileOut+"_test",r[-100:,:],fmt='%f')

    subset_list = [1000,10000,100000,1000000]
    for l in [2,8]:
        pca = PCA(n_components=l)
        mat = pca.fit_transform(data)
        for lst in subset_list:
            random_choices = np.random.choice(mat.shape[0],size = (lst),replace=False)
            np.savetxt(str(l) + "_" + str(lst),mat[random_choices[0:-100],:],fmt='%f')
            np.savetxt(str(l) + "_" + str(lst) + "_test",mat[random_choices[-100:],:],fmt='%f')
main()