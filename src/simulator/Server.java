import java.time.LocalDateTime;
import java.util.ArrayDeque;
import java.util.Queue;
import java.util.Random;

public class Server {
    private final ResourceType resourceType;
    private final double lambda;
    private final int serverID;

    long numberOfProcessedTasks = 0;
    long numberOfJobsSampled = 0;
    private Queue<Task> jobQueue;

    double workingTime = 0;

    private Task currentTask;

    private Random random;

    public Server(ResourceType resourceType, int serverID, double lambda) {
        this.resourceType = resourceType;
        this.lambda = lambda;
        this.serverID = serverID;

        this.random = new Random();
        this.random.setSeed(System.currentTimeMillis());

        this.jobQueue = new ArrayDeque<>();
    }

    private double getExponentialRandom() {
        /*switch (serverID)
        {
            case 0:
                return 0.005;
            case 1:
                return 0.020;
            case 2:
            case 3:
                return 0.015;
            default:
                return 0.020;
        }*/

        return -(Math.log(random.nextDouble()) / lambda);
    }

    public void ScheduleTask(Task task, Simulator simulator) {
        if (currentTask != null)
            jobQueue.add(task);
        else {
            double duration = getExponentialRandom();

            currentTask = task;
            workingTime += duration;

            Event newEvent = new Event(duration, simulator.simulationTime + duration, task, this);
            simulator.getEventQueue().add(newEvent);
        }
    }

    public void ExecuteTask(Simulator simulator) {
        ///////////////////////////////
        // JOB -> START
        ///////////////////////////////
        numberOfProcessedTasks++;
        //System.out.println("Server " + serverID + " has just executed task " + currentTask.taskID + ".");
        ///////////////////////////////
        // JOB -> END
        ///////////////////////////////

        // JOB IS FINISHED -> EXECUTE NEXT
        currentTask = null;
        if (!jobQueue.isEmpty()) {
            Task taskToSchedule = jobQueue.poll();
            ScheduleTask(taskToSchedule, simulator);
        }
    }

    public int getServerID() {
        return serverID;
    }

    public void updateStatistics() {
        numberOfJobsSampled += jobQueue.size();
        numberOfJobsSampled += (currentTask != null ? 1 : 0);
    }
}
