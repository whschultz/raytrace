package raytrace.engine;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import static org.junit.Assert.*;

/**
 * Created by whschultz on 6/11/15.
 */
public class laVectorTest {

    @Before
    public void setUp() throws Exception {

    }

    @After
    public void tearDown() throws Exception {

    }

    @Test
    public void testGet_x() throws Exception
    {
        laVector test = new laVector(1,2,3);

        assertEquals(1d, test.get_x(), .0000001);
    }

    @Test
    public void testGet_y() throws Exception {
        laVector test = new laVector(1,2,3);

        assertEquals(2d, test.get_y(), .0000001);
    }

    @Test
    public void testGet_z() throws Exception {
        laVector test = new laVector(1,2,3);

        assertEquals(3d, test.get_z(), .0000001);
    }

    @Test
    public void testEquals() throws Exception {
        assertEquals(new laVector(1,2,3), new laVector(1,2,3));
        assertNotEquals(new laVector(1,2,3), new laVector(3,2,1));
    }

    @Test
    public void testAdd() throws Exception {

    }

    @Test
    public void testSubtract() throws Exception {

    }

    @Test
    public void testNorm() throws Exception {

    }

    @Test
    public void testCross() throws Exception {

    }

    @Test
    public void testCross1() throws Exception {

    }

    @Test
    public void testDot() throws Exception {

    }

    @Test
    public void testDot1() throws Exception {

    }

    @Test
    public void testMultiply() throws Exception {

    }

    @Test
    public void testUnit() throws Exception {

    }
}