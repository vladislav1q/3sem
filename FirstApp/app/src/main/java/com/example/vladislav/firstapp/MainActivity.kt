package com.example.vladislav.firstapp

import android.os.Bundle
import android.support.v7.app.AppCompatActivity;
import android.content.Intent
import android.os.CountDownTimer
import android.os.Handler

var pressBack : Boolean = false

//Функцию печати чисел сам не смог придумать как написать, поэтому спросил у тех кто сделал
//Задержку через CountDownTimer в main activity не смог также реализовать, вторая активити почему-то не запускалась
//поэтому сделал через handler (как иначе не знаю)


class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        //val timer = MyCounter(4000, 1000)
        //timer.start()

        val intent = Intent(this, Main2Activity::class.java)
        Handler().postDelayed({
            if(!pressBack) startActivity(intent)
            pressBack = false
            finish()
        }, 2000)
    }

    /*fun strAct() {

        val intent = Intent(this, Main2Activity::class.java)
        startActivity(intent)

        //finish()
    }

    inner class MyCounter(millisInFuture: Long, countDownInterval: Long) : CountDownTimer(millisInFuture, countDownInterval) {
        override fun onFinish() {
            val to = MainActivity()
            to.strAct()
        }

        override fun onTick(millisUntilFinished: Long) {

        }
    }*/
}
