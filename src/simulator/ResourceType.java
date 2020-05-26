public enum ResourceType {
    CPU(0),
    SYSTEM_DISK_1(1),
    SYSTEM_DISK_2(2),
    SYSTEM_DISK_3(3),
    USER_DISK(4);

    public int val;

    ResourceType(int val) {
        this.val = val;
    }
}
