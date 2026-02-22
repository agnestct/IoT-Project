%% Linear Regression for Current Floor
% x = Pressure feature (Pa)
% y = Floor label (integer)

% --- Example data ---
% You can replace this with your actual measured pressure data and floor labels
%x = [101013.6, 100971.9, 100927.2, 100893.1, 100857.0 , 100824.9];  % Pressure
x = [99010, 98967, 98926, 98891, 98857, 98822];  % Pressure
y = [2, 3, 4, 5 , 6, 7];                            % Corresponding floors

% --- Use relative pressure ---
P_base = x(1);       % Use the pressure at the first floor as the baseline
x_rel = x - P_base;

% --- Least squares method to calculate w and b ---
x_mean = mean(x_rel);
y_mean = mean(y);

w = sum( (x_rel - x_mean) .* (y - y_mean) ) / sum( (x_rel - x_mean).^2 );
b = y_mean - w * x_mean;

fprintf('Linear regression parameters: w = %.5f, b = %.5f\n', w, b);

% --- Predict floor using the model ---
y_pred = w * x_rel + b;

fprintf('Predicted floors:\n');
disp(y_pred);
