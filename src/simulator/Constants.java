public class Constants {

    public static final double cpu = 1000. / 5;
    public static final double sys_disk1 = 1000. / 20;
    public static final double sys_disk2_3 = 1000. / 15;
    public static final double usr_disk = 1000. / 20;

    private final double[][] probabilityMatrix;

    public Constants(int K) {
        this.probabilityMatrix = new double[4 + K][4 + K];

        probabilityMatrix[0][0] = 0.1;  // CPU -> CPU
        probabilityMatrix[0][1] = 0.1;  // CPU -> SYS_DISK1
        probabilityMatrix[0][2] = 0.1;  // CPU -> SYS_DISK2
        probabilityMatrix[0][3] = 0.1;  // CPU -> SYS_DISK3

        probabilityMatrix[1][0] = 0.4;  // SYS_DISK1 -> CPU
        probabilityMatrix[2][0] = 0.4;  // SYS_DISK2 -> CPU
        probabilityMatrix[3][0] = 0.4;  // SYS_DISK3 -> CPU

        final double toUserDisk = (1 - 0.4) / K;
        final double toCPU = 1;

        for (int i = 0; i < 4 + K; i++) {
            for (int j = 4; i < 4 && j < 4 + K; j++)
                probabilityMatrix[i][j] = toUserDisk;

            if (i >= 4)
                probabilityMatrix[i][0] = toCPU;
        }
    }

    public double[][] getProbabilityMatrix() {
        return probabilityMatrix;
    }
}
