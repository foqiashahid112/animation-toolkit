com = dlmread("120_Nikki_SlowWalk.bvh-com.csv", ",", 1, 0);
comAcc = dlmread("120_Nikki_SlowWalk.bvh-comAcc.csv", ",", 1, 0);
forces = dlmread("120_Nikki_SlowWalk.bvh-forces.csv", ",", 1, 0);
torques = dlmread("120_Nikki_SlowWalk.bvh-moments.csv", ",", 1, 0);
platedata = dlmread("120_Nikki_SlowWalk.csv", ",", 18, 0);

numframesPlate = rows(platedata);
plateForces1 = platedata(1:8:numframesPlate, 8:10);
plateForces2 = platedata(1:8:numframesPlate, 20:22);
figure;
hold on;
plot(1:rows(plateForces1), plateForces1(:,1), 'r'); 
plot(1:rows(plateForces1), plateForces1(:,2), 'g');
plot(1:rows(plateForces1), plateForces1(:,3), 'b');

plot(1:rows(plateForces2), plateForces2(:,1), 'r'); 
plot(1:rows(plateForces2), plateForces2(:,2), 'g');
plot(1:rows(plateForces2), plateForces2(:,3), 'b');
hold off;

plateTorques1 = platedata(1:8:numframesPlate, 11:13);
plateTorques2 = platedata(1:8:numframesPlate, 23:25);
plateCPs1 = plateTorques2(1:8:numframesPlate, 1:25);

figure;
hold on;
plot(1:rows(plateTorques1), plateTorques1(:,1), 'r'); 
plot(1:rows(plateTorques1), plateTorques1(:,2), 'g');
plot(1:rows(plateTorques1), plateTorques1(:,3), 'b');

plot(1:rows(plateTorques2), plateTorques2(:,1), 'r'); 
plot(1:rows(plateTorques2), plateTorques2(:,2), 'g');
plot(1:rows(plateTorques2), plateTorques2(:,3), 'b');
hold off;

totalMass = 58.7714;
numframes = rows(com);
gravity = -9.8;
extForces = zeros(numframes, 3);
extForces(:,2) = gravity;

forceTest1 = (comAcc(:,1:3) - extForces) * totalMass;
torqueTest1 = cross(com(:,1:3), forces(:,1:3), 2);

figure;
hold on;
plot(1:numframes, forceTest1(:,1), "r");
plot(1:numframes, forceTest1(:,2), "g");
plot(1:numframes, forceTest1(:,3), "b");

plot(1:numframes, forces(:,1), "--r");
plot(1:numframes, forces(:,2), "--g");
plot(1:numframes, forces(:,3), "--b");

plot(1:rows(plateForces1), plateForces1(:,1), '..r'); 
plot(1:rows(plateForces1), plateForces1(:,2), '..g');
plot(1:rows(plateForces1), plateForces1(:,3), '..b');

plot(1:rows(plateForces2), plateForces2(:,1), '..r'); 
plot(1:rows(plateForces2), plateForces2(:,2), '..g');
plot(1:rows(plateForces2), plateForces2(:,3), '..b');

hold off;

figure;
hold on;
plot(1:numframes, torqueTest1(:,1), "r");
plot(1:numframes, torqueTest1(:,2), "g");
plot(1:numframes, torqueTest1(:,3), "b");

plot(1:numframes, torques(:,1), "--r");
plot(1:numframes, torques(:,2), "--g");
plot(1:numframes, torques(:,3), "--b");
hold off;