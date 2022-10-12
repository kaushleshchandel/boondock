<?php
include "_config.php";
 
if (isset($_GET['logout']))
    if($_GET['logout'] == "1")
        session_destroy();

if (isset($_POST['but_submit'])) {

    $uname = mysqli_real_escape_string($con, $_POST['txt_uname']);
    $password = mysqli_real_escape_string($con, $_POST['txt_pwd']);

    if ($uname != "" && $password != "") {

        $sql_query = "select 1 as cntUser, id, firstname, lastname, is_super_admin, is_admin from bdk.user where username='" . $uname . "' and password='" . $password . "'";
        $result = mysqli_query($con, $sql_query);
        $row = mysqli_fetch_array($result);

        $count = $row['cntUser'];


        if ($count > 0) {
            session_start();

            $_SESSION['uname'] = $uname;
            $_SESSION['id'] = $row['id'];
            $_SESSION['is_admin'] = $row['is_admin'];
            $_SESSION['is_super'] = $row['is_super_admin'];
            $_SESSION["fullname"] = $row['firstname'] . " " . $row['lastname'];
            header('Location: home.php');
        } else {
            echo "Invalid username and password";
        }
    }
}
?>

<!DOCTYPE html>
<html lang="en">

<head>

    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <meta name="description" content="">
    <meta name="author" content="">

    <title>Boondock Echo - Login</title>

    <!-- Custom fonts for this template-->
    <link href="vendor/fontawesome-free/css/all.min.css" rel="stylesheet" type="text/css">
    <link href="https://fonts.googleapis.com/css?family=Nunito:200,200i,300,300i,400,400i,600,600i,700,700i,800,800i,900,900i" rel="stylesheet">

    <!-- Custom styles for this template-->
    <link href="css/sb-admin-2.min.css" rel="stylesheet">

</head>

<body class="bg-gradient-primary">

    <div class="container">

        <!-- Outer Row -->
        <div class="row justify-content-center">

            <div class="col-xl-6 col-lg-12 col-md-9">

                <div class="card o-hidden border-0 shadow-lg my-5">
                    <div class="card-body p-0">
                        <!-- Nested Row within Card Body -->
                        <div class="row">

                            <div class="col-lg-12">
                                <div class="p-5">
                                    <div class="text-center">
                                        <h1 class="h4 text-gray-900 mb-4">Welcome to Boondock Echo!</h1>
                                    </div>
                                    <div class="container">
                                        <form method="post" action="">
                                            <div id="div_login">

                                                <div>
                                                    <input type="text" class="form-control form-control-user" class="textbox" id="txt_uname" name="txt_uname" placeholder="Username" />
                                                </div>
                                                <div>
                                                    <input type="password" class="form-control form-control-user" class="textbox" id="txt_uname" name="txt_pwd" placeholder="Password" />
                                                </div>
                                                <div>
                                                    <input type="submit" class="btn btn-primary btn-user btn-block" value="Submit" name="but_submit" id="but_submit" />
                                                </div>
                                            </div>
                                        </form>
                                    </div>


                                    <div class="text-center">
                                        <a class="small" href="forgot-password.html">Forgot Password?</a>
                                    </div>
                                    <div class="text-center">
                                        <a class="small" href="register.html">Create an Account!</a>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>

            </div>

        </div>

    </div>

    <!-- Bootstrap core JavaScript-->
    <script src="vendor/jquery/jquery.min.js"></script>
    <script src="vendor/bootstrap/js/bootstrap.bundle.min.js"></script>

    <!-- Core plugin JavaScript-->
    <script src="vendor/jquery-easing/jquery.easing.min.js"></script>

    <!-- Custom scripts for all pages-->
    <script src="js/sb-admin-2.min.js"></script>

</body>

</html>