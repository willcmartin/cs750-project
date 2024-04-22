import torch 
from sklearn.datasets import load_iris 
from torch.utils.data import TensorDataset, DataLoader 
from sklearn.model_selection import train_test_split 
import torch.nn as nn 
from torchinfo import summary 

# source: https://www.geeksforgeeks.org/multinomial-logistic-regression-with-pytorch/

class LogisticRegression(nn.Module): 
    def __init__(self, input_size, num_classes): 
        super(LogisticRegression, self).__init__() 
        self.linear = nn.Linear(input_size, num_classes) 
  
    def forward(self, x): 
        out = self.linear(x) 
        out = nn.functional.softmax(out, dim=1) 
        return out 
  
# Load the Iris dataset 
iris = load_iris() 
  
# Convert the data to PyTorch tensors 
X = torch.tensor(iris.data, dtype=torch.float32) 
y = torch.tensor(iris.target, dtype=torch.long) 
  
# Normalize the input data 
mean = torch.mean(X, dim=0) 
std = torch.std(X, dim=0) 
X = (X - mean) / std 
  
# Split the dataset into training and validation sets 
X_train, X_val, y_train, y_val = train_test_split(X, y, test_size=0.2, random_state=42) 
  
# Create PyTorch Datasets 
train_dataset = TensorDataset(X_train, y_train) 
val_dataset = TensorDataset(X_val, y_val) 
  
# Define the data loaders 
batch_size = 16
train_loader = DataLoader(train_dataset, batch_size=batch_size, shuffle=True) 
val_loader = DataLoader(val_dataset, batch_size=batch_size, shuffle=False)

# Define the model 
model = LogisticRegression(input_size=4, num_classes=3) 
  
# Check for cuda 
device = torch.device('cuda:0' if torch.cuda.is_available() else 'cpu') 
# Move the model to the device 
model = model.to(device) 
summary(model, input_size=(16,4))

# Define the loss function and optimizer 
criterion = nn.CrossEntropyLoss() 
optimizer = torch.optim.SGD(model.parameters(), lr=0.002)

# Define training parameters 
num_epochs = 1000
  
# Train the model 
for epoch in range(num_epochs): 
    for i, (inputs, labels) in enumerate(train_loader): 
        # Move inputs and labels to the device 
        inputs = inputs.to(device) 
        labels = labels.to(device) 
  
        # Forward pass 
        outputs = model(inputs) 
        loss = criterion(outputs, labels) 
  
        # Backward and optimize 
        optimizer.zero_grad() 
        loss.backward() 
        optimizer.step() 
  
    # Print training loss for each epoch 
    if (epoch+1)%100 == 0: 
        print('Epoch [{}/{}], Loss: {:.4f}'.format(epoch+1, num_epochs, loss.item()))

# Evaluate the model on the validation set 
with torch.no_grad(): 
    correct = 0
    total = 0
    for inputs, labels in val_loader: 
        # Move inputs and labels to the device 
        inputs = inputs.to(device) 
        labels = labels.to(device) 
  
        # Compute the model's predictions 
        outputs = model(inputs) 
        _, predicted = torch.max(outputs.data, 1) 
  
        # Compute the accuracy 
        total += labels.size(0) 
        correct += (predicted == labels).sum().item() 
  
    print('Validation Accuracy: {:.2f}%'.format(100 * correct / total))
