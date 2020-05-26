import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.PriorityQueue;
import java.util.Random;

public class Simulator {
    private final int n;
    private final int K;

    double simulationTime;

    private long totalJobCounter;

    private Server[] resources;
    private PriorityQueue<Event> eventQueue;

    private final double[][] probabilityMatrix;
    private Random random;

    Simulator(int n, int K) {
        this.n = n;
        this.K = K;

        this.eventQueue = new PriorityQueue<>(n, new EventComparator());
        this.probabilityMatrix = new Constants(K).getProbabilityMatrix();
        this.random = new Random();
        this.random.setSeed(System.currentTimeMillis());

        InitializeSystem();
    }

    private void InitializeSystem() {
        //////////////////////////
        // ALLOCATING RESOURCES //
        //////////////////////////
        resources = new Server[4 + K];

        resources[ResourceType.CPU.val] = new Server(ResourceType.CPU, 0, Constants.cpu);
        resources[ResourceType.SYSTEM_DISK_1.val] = new Server(ResourceType.SYSTEM_DISK_1, 1, Constants.sys_disk1);
        resources[ResourceType.SYSTEM_DISK_2.val] = new Server(ResourceType.SYSTEM_DISK_2, 2, Constants.sys_disk2_3);
        resources[ResourceType.SYSTEM_DISK_3.val] = new Server(ResourceType.SYSTEM_DISK_3, 3, Constants.sys_disk2_3);
        for (int i = 0; i < K; i++)
            resources[4 + i] = new Server(ResourceType.USER_DISK, 4 + i, Constants.usr_disk);

        //////////////////////////
        //   ALLOCATING TASKS   //
        //////////////////////////
        for (int i = 0; i < n; i++)
            resources[ResourceType.CPU.val].ScheduleTask(new Task(i), this);
    }

    PriorityQueue<Event> getEventQueue() {
        return eventQueue;
    }

    private int decideJumpTarget(int serverFrom) {
        double probabilitySum = 0;
        double jumpProbability = random.nextDouble();
        for (int i = 0; i < probabilityMatrix.length; i++) {
            probabilitySum += probabilityMatrix[serverFrom][i];

            if (jumpProbability < probabilitySum)
                return i;
        }

        return probabilityMatrix.length - 1;
    }

    public void RunSimulation(double shutdownTime) {
        while (simulationTime < shutdownTime) {
            // get topmost event with respect to time
            Event event = eventQueue.poll();

            // update simulation time
            simulationTime = event.endTime;

            // execute the task and schedule next job on that server
            event.server.ExecuteTask(this);
            // decide where task goes
            int jumpTarget = decideJumpTarget(event.server.getServerID());
            // put task into row on next server
            resources[jumpTarget].ScheduleTask(event.task, this);

            // update total job counter
            totalJobCounter++;
            // update statistics for calculating the average number of jobs
            for (int i = 0; i < 4 + K; i++)
                resources[i].updateStatistics();
        }
    }

    public void WriteSimulationResults(double simulationTime, String filename) {
        try (BufferedWriter bufferedWriter = new BufferedWriter(new FileWriter(filename, true))) {
            bufferedWriter.write("////////////////////////////////////////");
            bufferedWriter.write(System.lineSeparator());
            bufferedWriter.write("/////////// K := " + K + "; N := " + n + "; ///////////");
            bufferedWriter.write(System.lineSeparator());
            bufferedWriter.write("////////////////////////////////////////");
            bufferedWriter.write(System.lineSeparator());

            for (int i = 0; i < resources.length; i++) {
                double U = resources[i].workingTime / simulationTime;
                double X = resources[i].numberOfProcessedTasks / simulationTime;
                double n_avg = resources[i].numberOfJobsSampled * 1.0 / totalJobCounter;
                double R = n_avg / X;

                if (i == 0) {
                    bufferedWriter.write(System.lineSeparator());
                    bufferedWriter.write("T = " + n / X + " [s]");
                    bufferedWriter.write(System.lineSeparator());
                    bufferedWriter.write(System.lineSeparator());

                    String label = String.format("%-15s %-15s %-15s %-15s %-15s", "Server #", "U", "X [1/s]", "n_avg", "R [s]");
                    bufferedWriter.write(label);
                    bufferedWriter.write(System.lineSeparator());
                }

                String str = String.format("%-15d %-15f %-15f %-15f %-15f", i, U, X, n_avg, R);
                bufferedWriter.write(str);
                bufferedWriter.write(System.lineSeparator());
            }

            bufferedWriter.write(System.lineSeparator());
            bufferedWriter.write(System.lineSeparator());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        double simulationTime = 24 * 60 * 60;

        for (int i = 0; i < 3; i++) {
            int numberOfTasks;
            if (i == 0)
                numberOfTasks = 10;
            else if (i == 1)
                numberOfTasks = 15;
            else
                numberOfTasks = 20;

            try {
                Files.delete(Paths.get("rezultati_simulacija_" + numberOfTasks + ".txt"));
                Files.delete(Paths.get("rezultati_simulacija_usrednjeno_" + numberOfTasks + ".txt"));
            } catch (Exception ex) {

            }

            for (int k = 2; k <= 8; k++) {
                Simulator oneTimeSimulator = new Simulator(numberOfTasks, k);

                oneTimeSimulator.RunSimulation(simulationTime);
                oneTimeSimulator.WriteSimulationResults(simulationTime, "rezultati_simulacija_" + numberOfTasks + ".txt");
                System.out.println("Output written to rezultati_simulacija_" + numberOfTasks + ".txt for n = " + numberOfTasks + " and k = " + k);

                Simulator simulator = new Simulator(numberOfTasks, k);
                double simTime = simulationTime;
                if (numberOfTasks == 10)
                    simTime *= 10;
                else if (numberOfTasks == 15)
                    simTime *= 25;
                else
                    simTime *= 100;

                simulator.RunSimulation(simTime);
                simulator.WriteSimulationResults(simTime, "rezultati_simulacija_usrednjeno_" + numberOfTasks + ".txt");
                System.out.println("Output written to rezultati_simulacija_usrednjeno_" + numberOfTasks + ".txt for n = " + numberOfTasks + " and k = " + k);
            }
        }
    }
}
