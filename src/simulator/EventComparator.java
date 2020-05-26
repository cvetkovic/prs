import java.util.Comparator;

public class EventComparator implements Comparator<Event>
{
    @Override
    public int compare(Event o1, Event o2) {
        return Double.compare(o1.endTime, o2.endTime);
    }
}
