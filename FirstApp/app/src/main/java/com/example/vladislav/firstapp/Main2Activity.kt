package com.example.vladislav.firstapp

import android.os.Bundle
import android.os.CountDownTimer
import android.support.v7.app.AppCompatActivity;
import android.widget.Button
import android.widget.TextView

var state : Boolean = false
var count : Long = 0

class Main2Activity : AppCompatActivity() {
    val por1 = listOf("", "один ", "два ", "три ", "четыре ", "пять ", "шесть ", "семь ", "восемь ", "девять ")
    val por2 = listOf("", "один", "двадцать ", "тридцать ", "сорок ", "пятьдесят ", "шестьдесят ", "семьдесят ", "восемьдесят ", "девяносто ")
    val por1_5 = listOf("десять", "одиннадцать ", "двеннадцать ", "тринадцать ", "четырнадцать ", "пятнадцать ",
        "шестнадцать ", "семнадцать ", "восемнадцать ", "девятнадцать ")
    val por3 = listOf("", "сто ", "двести ", "триста ", "четыреста ", "пятьсот ", "шестьсот ", "семьсот ", "восемьсот ", "девятьсот ")


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main2)

        if(pressBack) finish()

        val button : Button = findViewById(R.id.button)
        val textView: TextView = findViewById(R.id.textView2)
        val timer = Mytimer(1000000, 1000)

        textView.text = "LET'S GO!"

        button.setOnClickListener {
            if(state){
                state = false
                button.text = "START"
                textView.text = "LET'S COUNT AGAIN!"
                count = 0
                timer.cancel()
            } else {
                state = true
                button.text = "STOP"
                timer.start()
            }
        }
    }

    fun setText(i: Long): String {
        var str: String

        if (i.toInt() == 0)
            str = "ноль"
        else if ((i % 100 / 10).toInt() == 1) {
            str = por3[(i / 100).toInt()]
            str = str.plus(por1_5[(i % 10).toInt()])
        } else {
            str = por3[(i / 100).toInt()]
            str = str.plus(por2[((i % 100) / 10).toInt()])
            str = str.plus(por1[(i % 10).toInt()])
        }

        return str
    }

    inner class Mytimer(millisInFuture: Long, countDownInterval: Long) : CountDownTimer(millisInFuture, countDownInterval) {
        val textView: TextView = findViewById(R.id.textView2)
        val button : Button = findViewById(R.id.button)

        override fun onFinish() {
            textView.text = "LET'S COUNT AGAIN!"
            button.text = "START"
        }

        override fun onTick(millisUntilFinished: Long) {
            textView.text = setText(count)
            count++
        }
    }

}
