package com.muye.bypasshiddenapi

import android.os.Bundle
import android.widget.Toast
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Button
import androidx.compose.material3.ButtonDefaults
import androidx.compose.material3.Card
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import com.muye.unsafereflect.ui.theme.BypassHiddenApiTheme

class MainActivity : ComponentActivity() {
    private val viewModel = BypassHiddenApiViewModel()
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {
            Content(viewModel)
        }
    }


    @Composable
    @Preview(showSystemUi = true)
    fun Content(viewModel: BypassHiddenApiViewModel? = null) {
        BypassHiddenApiTheme {
            Scaffold(modifier = Modifier.fillMaxSize()) { innerPadding ->
                Column(
                    modifier = Modifier
                        .fillMaxSize()
                        .padding(innerPadding),
                    horizontalAlignment = Alignment.CenterHorizontally,
                    verticalArrangement = Arrangement.spacedBy(24.dp, Alignment.CenterVertically)
                ) {
                    Card {
                        Column(modifier = Modifier.padding(12.dp)
                            .align(Alignment.CenterHorizontally)) {
                            Button(onClick = {
                                if(viewModel?.bypassByModifyAccessFlag() == true){
                                    Toast.makeText(this@MainActivity, "success", Toast.LENGTH_SHORT).show()
                                }else{
                                    Toast.makeText(this@MainActivity, "fail", Toast.LENGTH_SHORT).show()
                                }
                            }) {
                                Text("Bypass By ModifyAccessFlag")
                            }
                            Button(onClick = {
                                if(viewModel?.bypassByGetMemberActionImpl() == true){
                                    Toast.makeText(this@MainActivity, "success", Toast.LENGTH_SHORT).show()
                                }else{
                                    Toast.makeText(this@MainActivity, "fail", Toast.LENGTH_SHORT).show()
                                }
                            }) {
                                Text("Bypass By GetMemberActionImpl")
                            }
                            Button(onClick = {
                                if(viewModel?.bypassByModifyEnforcementPolicy(this@MainActivity) == true){
                                    Toast.makeText(this@MainActivity, "success", Toast.LENGTH_SHORT).show()
                                }else{
                                    Toast.makeText(this@MainActivity, "fail", Toast.LENGTH_SHORT).show()
                                }
                            }) {
                                Text("Bypass By ModifyEnforcementPolicy")
                            }
                            Button(onClick = {
                                if(viewModel?.bypassByNullClassLoader(this@MainActivity) == true){
                                    Toast.makeText(this@MainActivity, "success", Toast.LENGTH_SHORT).show()
                                }else{
                                    Toast.makeText(this@MainActivity, "fail", Toast.LENGTH_SHORT).show()
                                }
                            }) {
                                Text("Bypass By NullClassLoader")
                            }
                            Button(onClick = {
                                if(viewModel?.bypassByMetaReflect() == true){
                                    Toast.makeText(this@MainActivity, "success", Toast.LENGTH_SHORT).show()
                                }else{
                                    Toast.makeText(this@MainActivity, "fail", Toast.LENGTH_SHORT).show()
                                }
                            }) {
                                Text("Bypass By MetaReflect")
                            }
                            Button(onClick = {
                                if(viewModel?.bypassByJniOnLoad() == true){
                                    Toast.makeText(this@MainActivity, "success", Toast.LENGTH_SHORT).show()
                                }else{
                                    Toast.makeText(this@MainActivity, "fail", Toast.LENGTH_SHORT).show()
                                }
                            }) {
                                Text("Bypass By Jni On Load")
                            }
                            Button(onClick = {
                                if(viewModel?.bypassByModifyHiddenApiExemptions() == true){
                                    Toast.makeText(this@MainActivity, "success", Toast.LENGTH_SHORT).show()
                                }else{
                                    Toast.makeText(this@MainActivity, "fail", Toast.LENGTH_SHORT).show()
                                }
                            }) {
                                Text("Bypass By ModifyHiddenApiExemptions")
                            }
                            Button(onClick = {
                                if(viewModel?.bypassByUnsafeReflect() == true){
                                    Toast.makeText(this@MainActivity, "success", Toast.LENGTH_SHORT).show()
                                }else{
                                    Toast.makeText(this@MainActivity, "fail", Toast.LENGTH_SHORT).show()
                                }
                            }) {
                                Text("Bypass By UnsafeReflect")
                            }
                        }
                    }
                    Card {
                        Column(modifier = Modifier.padding(12.dp)
                            .align(Alignment.CenterHorizontally)) {
                            Button(onClick = {
                                if(viewModel?.callHiddenApi() == true){
                                    Toast.makeText(this@MainActivity, "call hidden api success", Toast.LENGTH_SHORT).show()
                                }else{
                                    Toast.makeText(this@MainActivity, "call hidden api fail", Toast.LENGTH_SHORT).show()
                                }
                            }, colors = ButtonDefaults.elevatedButtonColors()) {
                                Text("Call Hidden Api")
                            }
                        }
                    }
                }
            }
        }
    }
}