package cn.edu.wust.calculator;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import java.util.ArrayList;

public class MainActivity extends AppCompatActivity implements View.OnClickListener
{

    // Used to load the 'native-lib' library on application startup.
    static
    {
        System.loadLibrary("native-lib");
    }

    TextView tv_expression;
    TextView tv_answer;

    int[] ids = new int[]
    {
        R.id.bt_sinh, R.id.bt_cosh, R.id.bt_tanh, R.id.bt_csc, R.id.bt_sec, R.id.bt_cot,
        R.id.bt_sin, R.id.bt_asin, R.id.bt_square_root, R.id.bt_all_clear, R.id.bt_clear,
        R.id.bt_divide, R.id.bt_cos, R.id.bt_acos, R.id.bt_one, R.id.bt_two, R.id.bt_three,
        R.id.bt_ride, R.id.bt_tan, R.id.bt_atan, R.id.bt_four, R.id.bt_five, R.id.bt_six,
        R.id.bt_minus, R.id.bt_l_brackets, R.id.bt_r_brackets, R.id.bt_seven, R.id.bt_eight,
        R.id.bt_nine, R.id.bt_plus, R.id.bt_log, R.id.bt_pi, R.id.bt_e, R.id.bt_zero,
        R.id.bt_point, R.id.bt_equal,R.id.bt_pow
    };

    String expression_a = new String();
    String expression_b = new String();
    String answer = new String();
    ArrayList<String> expression_a_list = new ArrayList<>();
    ArrayList<String> expression_b_list = new ArrayList<>();

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        initView();
    }

    private void initView()
    {
        tv_expression = findViewById(R.id.tv_expression);
        tv_answer = findViewById(R.id.tv_answer);

        for (int id : ids)
        {
            findViewById(id).setOnClickListener(this);
        }
    }

    @Override
    public void onClick(View v)
    {
        switch (v.getId())
        {
            case R.id.bt_sinh:
                expression_a_list.add("sinh(");
                expression_b_list.add("K(");
                setExpression_a();

                break;

            case R.id.bt_cosh:
                expression_a_list.add("cosh(");
                expression_b_list.add("L(");
                setExpression_a();

                break;

            case R.id.bt_tanh:
                expression_a_list.add("tanh(");
                expression_b_list.add("M(");
                setExpression_a();

                break;

            case R.id.bt_csc:
                expression_a_list.add("csc(");
                expression_b_list.add("H(");
                setExpression_a();

                break;

            case R.id.bt_sec:
                expression_a_list.add("sec(");
                expression_b_list.add("I(");
                setExpression_a();

                break;

            case R.id.bt_cot:
                expression_a_list.add("cot(");
                expression_b_list.add("J(");
                setExpression_a();

                break;

            case R.id.bt_sin:
                expression_a_list.add("sin(");
                expression_b_list.add("S(");
                setExpression_a();

                break;

            case R.id.bt_asin:
                expression_a_list.add("asin(");
                expression_b_list.add("B(");
                setExpression_a();

                break;

            case R.id.bt_square_root:
                expression_a_list.add("√");
                expression_b_list.add("G");
                setExpression_a();

                break;

            case R.id.bt_all_clear:
                expression_a_list = new ArrayList<>();
                expression_b_list = new ArrayList<>();
                setExpression_a();
                tv_answer.setTextSize(32);
                tv_answer.setText("0");

                break;

            case R.id.bt_clear:
                if (expression_a_list.size() > 0)
                {
                    expression_a_list.remove(expression_a_list.size() - 1);
                    expression_b_list.remove(expression_b_list.size() - 1);
                    setExpression_a();
                }

                break;

            case R.id.bt_divide:
                expression_a_list.add("/");
                expression_b_list.add("/");
                setExpression_a();

                break;

            case R.id.bt_cos:
                expression_a_list.add("cos(");
                expression_b_list.add("C(");
                setExpression_a();

                break;

            case R.id.bt_acos:
                expression_a_list.add("acos(");
                expression_b_list.add("D(");
                setExpression_a();

                break;

            case R.id.bt_one:
                expression_a_list.add("1");
                expression_b_list.add("1");
                setExpression_a();

                break;

            case R.id.bt_two:
                expression_a_list.add("2");
                expression_b_list.add("2");
                setExpression_a();

                break;

            case R.id.bt_three:
                expression_a_list.add("3");
                expression_b_list.add("3");
                setExpression_a();

                break;

            case R.id.bt_ride:
                expression_a_list.add("*");
                expression_b_list.add("*");
                setExpression_a();

                break;

            case R.id.bt_tan:
                expression_a_list.add("tan(");
                expression_b_list.add("T(");
                setExpression_a();

                break;

            case R.id.bt_atan:
                expression_a_list.add("atan(");
                expression_b_list.add("E(");
                setExpression_a();

                break;

            case R.id.bt_four:
                expression_a_list.add("4");
                expression_b_list.add("4");
                setExpression_a();

                break;

            case R.id.bt_five:
                expression_a_list.add("5");
                expression_b_list.add("5");
                setExpression_a();

                break;

            case R.id.bt_six:
                expression_a_list.add("6");
                expression_b_list.add("6");
                setExpression_a();

                break;

            case R.id.bt_minus:
                expression_a_list.add("-");
                expression_b_list.add("-");
                setExpression_a();

                break;

            case R.id.bt_l_brackets:
                expression_a_list.add("(");
                expression_b_list.add("(");
                setExpression_a();

                break;

            case R.id.bt_r_brackets:
                expression_a_list.add(")");
                expression_b_list.add(")");
                setExpression_a();

                break;

            case R.id.bt_log:
                expression_a_list.add("lg(");
                expression_b_list.add("A(");
                setExpression_a();

                break;

            case R.id.bt_seven:
                expression_a_list.add("7");
                expression_b_list.add("7");
                setExpression_a();

                break;

            case R.id.bt_eight:
                expression_a_list.add("8");
                expression_b_list.add("8");
                setExpression_a();

                break;

            case R.id.bt_nine:
                expression_a_list.add("9");
                expression_b_list.add("9");
                setExpression_a();

                break;

            case R.id.bt_plus:
                expression_a_list.add("+");
                expression_b_list.add("+");
                setExpression_a();

                break;

            case R.id.bt_pi:
                expression_a_list.add("π");
                expression_b_list.add("p");
                setExpression_a();

                break;

            case R.id.bt_e:
                expression_a_list.add("e");
                expression_b_list.add("e");
                setExpression_a();

                break;

            case R.id.bt_zero:
                expression_a_list.add("0");
                expression_b_list.add("0");
                setExpression_a();

                break;

            case R.id.bt_point:
                expression_a_list.add(".");
                expression_b_list.add(".");
                setExpression_a();

                break;

            case R.id.bt_pow:
                expression_a_list.add("^");
                expression_b_list.add("^");
                setExpression_a();

                break;

            case R.id.bt_equal:
                setExpression_a();
                doCalculation();

                break;
        }

    }

    private void doCalculation()
    {
        answer = "";
        expression_b = "";

        for (String i : expression_b_list)
        {
            expression_b += i;
        }

        if(expression_b.length() < 1)
        {
            expression_b = "0";
        }

        answer = answer + Calculator_Algorithm(expression_b + "=");

        if(answer.length() > 30)
        {
            tv_answer.setTextSize(12);
        }

        else if(answer.length() > 26)
        {
            tv_answer.setTextSize(15);
        }

        else if(answer.length() > 20)
        {
            tv_answer.setTextSize(20);
        }

        else
        {
            tv_answer.setTextSize(32);
        }

        tv_answer.setText(answer);
    }


    public void setExpression_a()
    {
        expression_a = "";

        for (String i : expression_a_list)
        {
            expression_a += i;
        }

        if(expression_a.length() < 10)
        {
            tv_expression.setTextSize(48);
        }

        if(expression_a.length() > 10)
        {
            tv_expression.setTextSize(36);
        }

        if(expression_a.length() > 28)
        {
            tv_expression.setTextSize(18);
        }

        if(expression_a.length() > 128)
        {
            tv_expression.setTextSize(9);
        }

        tv_expression.setText(expression_a);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */

    public native String Calculator_Algorithm(String t);
}
