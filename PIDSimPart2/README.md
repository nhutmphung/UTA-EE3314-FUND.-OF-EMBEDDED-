Final graph of serial plotter CIenTi of control function: 

<img src="EE3314\PIDSimPart2\PIDSim\PIDSIMpart2.png" width="400">

Chosen values were: 

Kp = 0.6;
Ki = 0.5;
Kd = 0.4;

Matlab simulation graph: 

<img src="EE3314\PIDSimPart2\PIDSim\matlabPIDSim.png" width="400">

matlab code:

%% Project 2 - Submission 2 MATLAB Simulation
% Plant:
% G(s) = 100 / (s^3 + 8s^2 + 17s + 10)
%
% PID Controller:
% D(s) = Kp + Ki/s + Kds

clear; clc; close all;

%% Plant
s = tf('s');

G = 100/(s^3 + 8s^2 + 17s + 10);

%% PID gains
% These gains were chosen by testing different values in MATLAB
% and selecting a response with faster settling and small overshoot.

Kp = 0.6;
Ki = 0.5;
Kd = 0.4;

D = Kp + Ki/s + Kds;

%% Closed-loop system
Tclosed = feedback(DG, 1);

%% Open-loop comparison
% The plant has a DC gain of 10, so the open-loop input is scaled by 0.1.
% This makes the open-loop final value close to 1.

Gopen_scaled = 0.1G;

%% Plot responses
figure;
step(Gopen_scaled, 10);
hold on;
step(Tclosed, 10);
grid on;

title('Open-Loop and PID Closed-Loop Step Response');
xlabel('Time (s)');
ylabel('Output y(t)');
legend('Open Loop, scaled input', 'PID Closed Loop', 'Location', 'best');

%% Display results
disp('Selected PID Gains:')
fprintf('Kp = %.3f\n', Kp);
fprintf('Ki = %.3f\n', Ki);
fprintf('Kd = %.3f\n\n', Kd);

disp('Closed-loop transfer function:')
Tclosed

disp('Closed-loop poles:')
pole(Tclosed)

disp('Closed-loop step response information:')
stepinfo(Tclosed)
