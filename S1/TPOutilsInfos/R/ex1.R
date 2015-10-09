lena = read.csv("Documents/M2GICAO/OutilsInfos/R/lena.csv",header = FALSE)
lena <- as.matrix(lena)
image(lena)
image(lena,col = gray.colors(256))
image(lena,col = gray.colors(256, start = 0, end=1, gamma=3.3, alpha = NULL))
image(lena,col = gray.colors(256, start = 0, end=1, gamma=0.5, alpha = NULL))

#ROTATION PAR 90 degres
rotation_qvd <- function(im){
  
  n <- dim(im)[1]
  m <- dim(im)[2]
  image <- as.matrix(im)
  res <- t(image)
  res2 <- matrix(0,n,m)
  for(i in 1:m){
    res2[,i] <- res[,((m+1)-i)]
    
  }
  return (res2)
}

#ROTATION par un angle multiple de 90
rotation <- function(image,angle){
  n <- angle%/%90
  im <- image
  for(i in 1:n){
    im <- rotation_qvd(im)
  }
  return(im)
}

#quantize affiche l'image en 2^k nuances de gris
quantize <- function(im,k){
  if( k>=1 && k<=8){
    image(im,col = gray.colors(2^k))    
  }
}

#reduction 
reduction <- function(im,k){
  n <- dim(im)[1]
  m <- n%/%k
  res <- matrix(0,m,m)
 for(i in 1:m){
   for(j in 1:m){
     res[i,j] <- im[i*k,j*k]
   }
 } 
 image(res)
 return(res)
}

#SVD compression
compression_SVD <- function(im,k){
  n <- dim(im)[1]
  m <- dim(im)[2]
  im_svd <- svd(im)
  im_svd_d <- im_svd$d
  im_svd_u <- im_svd$u
  im_svd_v <- im_svd$v
  
  #On doit prendre les k premieres valeurs de d
  d_final <- im_svd_d[1:k]
  #On transforme le vecteur d en matrice qui a pour diagonale les valeurs de d
  Md <- matrix(0,k,k)
  diag(Md) <- d_final
  #On prend la sous matrice de dimension n x k de u ATTENTION M COLONNES !!
  Mu <- im_svd_u[seq(1,n),seq(1,k)]
  #On prend la sous matrice de dimension m x k de v ATTENTIOOOON N LIGNES !!
  Mv <- im_svd_v[seq(1,m),seq(1,k)]
  #dimension avant troncage
  dim(im_svd_u)
  #dimension apres troncage
  dim(Mu)
  #On transpose v
  Mv <- t(Mv)
  
  #On fait u *d * t(v) pour avoir A^k 
  Ak <- (Mu %*% Md) %*% Mv
  return(Ak)
}

#Image denoising
#bruiter une image
bruiter <- function(im,k){
  #on construit la matrice de bruit
  mb <- matrix(rnorm(dim(im)[1]*dim(im)[2],mean=0,sd=k),dim(im)[1],dim(im)[2])
  #on l'applique à l'image
  Mc <- mb + im
  image(Mc)
  return (Mc)
}

#debruiter une image
debruiter <- function(im,s,methode){
  #On construit le noyau de convolution
  k <- sqrt(s)
  k2 <- sqrt(s) %/% 2
  if(s%%2 ==0){#si s pair
    ker <- matrix(1,k+1,k+1)
    ker[,k+1] <-0
    ker[k+1,] <-0
    k <- k+1
    k2 <-k %/%2
  }
  if(methode == 1){#moyenneur
    ker <- matrix(1,k,k)
  }else{#gauss
    if(s==9){
      ker <- matrix(c(1,2,1,2,4,2,1,2,1),nrow = k)      
    }else if(s == 25){
      ker <- matrix(c(1,7,16,7,1,7,75,164,75,7,16,164,359,164,16,7,75,164,75,7,1,7,16,7,1),nrow = k)
      
    }
    
  }
  
  mat <- im
  #On duplique les bords
  for( i in 1:k2){
    mat <- cbind(mat,mat[,dim(mat)[1]])
    mat <- cbind(mat[, 1],mat)
    mat <- rbind(mat,mat[dim(mat)[1],])
    mat <- rbind(mat[1,],mat)
  }
  #copie de mat
  res <-mat
  n <- dim(mat)[1]
  #On applique le noyau de convolution
    
    for( i in (k2+1):(n-k2)){
      for(j in (k2+1):(n-k2)){
        
        #recuperer sous matrice de centre i,j
        sm <- mat[,c((i-k2):(i+k2))]
        sm <- sm[c((j-k2):(j+k2)),]
        
        dim(sm)
        dim(ker)
        #multiplier avec ker
        multmat <- sm*ker
        #additioner les elements de la matrice resultat
        
        #stocker ds  res a i,j
        res[j,i] <- sum(multmat) %/% sum(ker) 
      }
    }
  
  
  #On enleve les lignes et colonnes de 0 precedemment ajoutées
  
  #On affiche
  return(res)
  
}

res <-matrix(0,dim(lena)[1],dim(lena)[2])
image(lena)
res<- rotation_qvd(lena)
res <- rotation(lena,180)
res <- rotation(lena,270)
image(res)
lena <- rotation_qvd(lena)
image(lena)
quantize(lena,9)
reduction(lena,100)


Ak <- compression_SVD(lena,10)

image(Ak)

Mdist <- dist(Ak, method = "euclidean")
Mdist <- as.matrix(Mdist)
image(Mdist)

Mdist <- dist(lena, method = "euclidean")
Mdist <- as.matrix(Mdist)
image(Mdist)
image(lena)

object.size(lena)
object.size(Ak)

#lena bruitee avec sd = 2
Mb <- bruiter(lena,2)

Mb <- bruiter(lena,5.0)

Mb <- bruiter(lena,10)

Mb <- bruiter(lena,25)

Mb <- bruiter(lena,50)


Mdb <-debruiter(Mb,16,1)

image(Mdb)
Mdist <- dist(Mdb, method = "euclidean")
Mdist <- as.matrix(Mdist)
image(Mdist)
