import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.stream.IntStream;

// Press Shift twice to open the Search Everywhere dialog and type `show whitespaces`,
// then press Enter. You can now see whitespace characters in your code.
public class Main {
    public static int[][] generateMatrix(int rows, int cols) {
        Random random = new Random();
        int[][] matrix = new int[rows][cols];
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                matrix[i][j] = random.nextInt(10); // Числа от 0 до 9
            }
        }
        return matrix;
    }
    public static void execotorSevice(int[][] matrix) throws InterruptedException, ExecutionException {

        long startTime = System.currentTimeMillis();
        int rows = 10000;
        int cols = 1000;
        ExecutorService executor = Executors.newFixedThreadPool(Runtime.getRuntime().availableProcessors());
        int numThreads = Runtime.getRuntime().availableProcessors();
        System.out.println("Количество потоков: "+ numThreads);
        int chunkSize = (int) Math.ceil((double) cols / numThreads);

        List<Future<Integer>> futures = new ArrayList<>();
        for (int i = 0; i < numThreads; i++) {
            final int start = i * chunkSize;
            final int end = Math.min((i + 1) * chunkSize, cols);
            futures.add(executor.submit(() -> find(matrix, start, end)));
        }

        int maxColumn = -1;
        int maxLength = -1;
        for (Future<Integer> future : futures) {
            int column = future.get();
            int length = calculateSequenceLength(matrix, column);
            if (length > maxLength) {
                maxLength = length;
                maxColumn = column;
            }
        }

        executor.shutdown();
        long endTime = System.currentTimeMillis();
        System.out.println("ExecutorService: Самая длинная серия в столбце " + maxColumn + ", длина " + maxLength);
        System.out.println("Время выполнения: " + (endTime - startTime) + " мс");
    }


    private static int find(int[][] matrix, int start, int end) {
        int maxColumn = -1;
        int maxLength = -1;
        for (int j = start; j < end; j++) {
            int length = calculateSequenceLength(matrix, j);
            if (length > maxLength) {
                maxLength = length;
                maxColumn = j;
            }
        }
        return maxColumn;
    }

    private static int calculateSequenceLength(int[][] matrix, int col) {
        int maxLength = 1;
        int currentLength = 1;
        for (int i = 1; i < matrix.length; i++) {
            if (matrix[i][col] == matrix[i - 1][col]) {
                currentLength++;
            } else {
                maxLength = Math.max(maxLength, currentLength);
                currentLength = 1;
            }
        }
        maxLength = Math.max(maxLength, currentLength); // Проверка на конец
        return maxLength;
    }
    public static void streams(int[][] matrix) {


        long startTime = System.currentTimeMillis();

        int maxColumn = IntStream.range(0, matrix[0].length)
                .parallel()
                .mapToObj(col -> new int[]{col, calculateSequenceLength(matrix, col)})
                .max((a, b) -> Integer.compare(a[1], b[1]))
                .map(arr -> arr[0])
                .orElse(-1);

        int maxLength = calculateSequenceLength(matrix, maxColumn);

        long endTime = System.currentTimeMillis();
        System.out.println("Streams: Самая длинная серия в столбце " + maxColumn + ", длина " + maxLength);
        System.out.println("Время выполнения: " + (endTime - startTime) + " мс");
    }
    public static void main(String[] args) throws ExecutionException, InterruptedException {
        int rows = 10000;
        int cols = 1000;
        int[][] matrix = generateMatrix(rows, cols);
        streams(matrix);
        execotorSevice(matrix);
    }
}