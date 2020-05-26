public class Event {
    public final double startTime;
    public final double endTime;
    public Task task;
    public Server server;

    public Event(double startTime, double endTime, Task task, Server server) {
        this.startTime = startTime;
        this.endTime = endTime;
        this.task = task;
        this.server = server;
    }
}
