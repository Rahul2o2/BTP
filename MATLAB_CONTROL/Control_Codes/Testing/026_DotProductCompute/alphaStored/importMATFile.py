import scipy.io

# Load the .mat file
mat_data = scipy.io.loadmat('your_file.mat')

# Now you can access variables from the .mat file
# For example, if you have a variable named 'data' in the .mat file:
if 'data' in mat_data:
    data = mat_data['data']
    print(data)
else:
    print("Variable 'data' not found in the .mat file.")
