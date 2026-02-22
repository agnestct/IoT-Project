%% Train Linear Regression from Arduino CSV (IMU only or with pressure)
% Assumes CSV has header: timestamp,pressure,temperature,accX,accY,accZ,accMagnitude

% --- Load CSV ---
filename = 'arduino_data.csv';  % replace with your CSV file
data = readtable(filename);

% --- Extract IMU data ---
accX = data.accX;
accY = data.accY;
accZ = data.accZ;

% --- Choose target ---
% Option 1: Predict floor
floor = data.floor;  % if you already logged floor labels manually

% Option 2: Predict movement (0 = idle, 1 = moving)
% movement = data.pattern;  

% --- Feature matrix ---
X = [accX, accY, accZ, aMag];  % only IMU
% If you want to add pressure too:
% X = [data.pressure, accX, accY, accZ, aMag];

% --- Add bias column ---
X_aug = [ones(size(X,1),1), X];

% --- Train linear regression ---
theta = (X_aug' * X_aug) \ (X_aug' * floor);

% --- Predict ---
y_pred = X_aug * theta;

% --- Display results ---
fprintf('Regression coefficients (bias + features):\n');
disp(theta);

fprintf('Predicted floors (or movement states):\n');
disp(y_pred);